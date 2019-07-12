#include "wolf3d.h"

void		player_debug(t_app *app)
{
	const Uint8 *key;

	key = app->inputs->keyboard;
//	if (key[SDL_SCANCODE_EQUALS] &&
//		app->inputs->sensitivity < 5)
//		app->inputs->sensitivity += 0.1;
//	if (key[SDL_SCANCODE_MINUS] &&
//			app->inputs->sensitivity > 0)
//		app->inputs->sensitivity -= 0.1;
//	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
//	{
//		if (app->inputs->sensitivity < 0)
//			app->inputs->sensitivity = 0;
//		if (app->inputs->sensitivity > 5)
//			app->inputs->sensitivity = 5;
//		printf("sensitivity -> %f\n", app->inputs->sensitivity);
//	}

	if (key[SDL_SCANCODE_EQUALS] &&
		app->player->obj_dist < (64 * 5))
		app->player->obj_dist += 5;
	if (key[SDL_SCANCODE_MINUS] &&
		app->player->obj_dist > 0)
		app->player->obj_dist -= 5;
	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
	{
		if (app->player->obj_dist < 0)
			app->player->obj_dist = 0;
		if (app->player->obj_dist > 64 * 5)
			app->player->obj_dist = 64 * 5;
		printf("obj_dist -> %f\n", app->player->obj_dist);
	}
}

void		on_mouse_update(t_app *app)
{
	app->inputs->left_pressed =
			SDL_GetRelativeMouseState(&app->inputs->x, &app->inputs->y) &
			SDL_BUTTON(SDL_BUTTON_LEFT);
	if (app->inputs->x)
	{
		app->player->direction += app->inputs->x *
				app->inputs->sensitivity * app->time->delta;
		if (app->player->direction > 359)
			app->player->direction = 0;
		if (app->player->direction < 0)
			app->player->direction = 359;
		app->player->x_v = cos(app->player->direction * M_PI_180);
		app->player->y_v = sin(app->player->direction * M_PI_180);
	}
	if (app->inputs->y)
		app->player->head_angle += app->inputs->y * 0.1 * app->time->delta;
}

void		start_the_game(t_app *app)
{
	init_time(app);
	printf("SetRelativeMouseMode -> %d\n", SDL_SetRelativeMouseMode(SDL_TRUE));
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	while (1)
	{
		update_time(app);
		SDL_PollEvent(&app->sdl->event);
		if (check_for_quit(app) == 1)
			break ;
		//player_debug(app);
		on_mouse_update(app);
		keyboard_input(app, app->time->frame);
		update_doors(app, app->time->frame);
		reset_objects(app->map);
		create_field_of_view(app);
		update_objects(app);
		redraw(app->sdl, app->player, app->time);
	}
}

void		load_level(t_app *app, int level)
{
	int		fd;
	char	*level_char;
	char	level_path[50];

	ft_strdel(&app->player->cur_level);
	level_char = ft_itoa(level);
	app->player->cur_level = level_char;
	ft_strcpy(level_path, "../levels/");
	ft_strcat(level_path, level_char);
	ft_strcat(level_path, ".wolf3d");
	if ((fd = open(level_path, O_RDONLY)) != -1)
		map_read(fd, app->map, app->player);
	else
		ft_error("Map path error.");
}

int			main(void)
{
	t_app	app;

	init(&app);
	player_init(app.sdl, app.player);
	load_level(&app, 1);
	start_the_game(&app);
	quit_properly(&app);
	return (0);
}
