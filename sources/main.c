#include "wolf3d.h"

void		player_debug(t_app *app)
{
	const Uint8 *key;
	//test comments3
	key = app->inputs->keyboard;
//	if (key[SDL_SCANCODE_EQUALS] &&
//		app->inputs->sensetivity < 5)
//		app->inputs->sensetivity += 0.1;
//	if (key[SDL_SCANCODE_MINUS] &&
//			app->inputs->sensetivity > 0)
//		app->inputs->sensetivity -= 0.1;
//	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
//	{
//		if (app->inputs->sensetivity < 0)
//			app->inputs->sensetivity = 0;
//		if (app->inputs->sensetivity > 5)
//			app->inputs->sensetivity = 5;
//		printf("sensetivity -> %f\n", app->inputs->sensetivity);
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
	if (app->inputs->x != 0)
	{
		app->player->direction += app->inputs->x *
				app->inputs->sensetivity * app->time->delta;
		if (app->player->direction > 359)
			app->player->direction = 0;
		if (app->player->direction < 0)
			app->player->direction = 359;
		app->player->x_v = cos(app->player->direction * M_PI_180);
		app->player->y_v = sin(app->player->direction * M_PI_180);
	}
}

void		start_the_game(t_app *app)
{
	float		dist;

	init_time(app);
	printf("SetRelativeMouseMode -> %d\n", SDL_SetRelativeMouseMode(SDL_TRUE));
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	while (1)
	{
		update_time(app);
		SDL_PollEvent(&app->sdl->event);
		if (check_for_quit(app) == 1)
			break ;
		player_debug(app);
		on_mouse_update(app);
		keyboard_input(app, app->time->frame);
		update_doors(app, app->time->frame);
		create_field_of_view(app);
		dist = sqrtf(ft_powf(app->player->x - app->map->objects[0]->center.x, 2) + ft_powf(app->player->y - app->map->objects[0]->center.y, 2));
		dist = (int)(64 / dist * app->sdl->dist_to_pp);
		draw_object(app, app->map->objects[0], dist);
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
