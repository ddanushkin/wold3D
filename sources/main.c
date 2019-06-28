#include "wolf3d.h"

void		player_debug(const Uint8 *key, t_player *player)
{
	if (key[SDL_SCANCODE_EQUALS] && player->health < 100)
		player->health++;
	if (key[SDL_SCANCODE_MINUS] && player->health > 0)
	{
		player->health--;
		if (player->health == 0)
			Mix_PlayChannel(-1, player->fx_die, 0);
	}
	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
		printf("%d\n", 6 - ((player->health) / 14) % 7);
}

float		angle_mapping(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (float)(x - in_min) * (float)(out_max - out_min) / (float)(in_max - in_min) + (float)out_min;
}

void		on_mouse_update(t_app *app)
{
	int		old_x;
	int		old_y;

	old_x = app->inputs->x;
	old_y = app->inputs->y;
	SDL_GetMouseState(&app->inputs->x, &app->inputs->y);
	if (old_x != app->inputs->x || old_y != app->inputs->y)
	{
		app->player->head_angle = angle_mapping(app->inputs->y, 0, app->sdl->height - app->sdl->height / 5, -10, 10);

//		if (old_x < app->inputs->x)
//			(app->player->direction += app->player->speed * 0.5) > 359 ? app->player->direction = 1 : 0;
//		if (old_x > app->inputs->x)
//			(app->player->direction -= app->player->speed * 0.5) < 0 ? app->player->direction = 359 : 0;
//		app->player->x_v = cos(app->player->direction * M_PI_180);
//		app->player->y_v = sin(app->player->direction * M_PI_180);
		printf("mouse - x: %d\nmouse - y: %d\n", app->inputs->x, app->inputs->y);
	}
}

void		start_the_game(t_app *app)
{
	t_time		time;

	init_time(&time);
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	while (1)
	{
		if (check_for_quit(app->sdl, app->inputs->keyboard) == 1)
			break ;
		on_mouse_update(app);
		update_time(&time, app);
		player_debug(app->inputs->keyboard, app->player);
		keyboard_input(app, app->inputs->keyboard, time.frame);
		update_doors(app, time.frame);
		create_field_of_view(app);
		redraw(app->sdl, app->player, &time);
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
