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

void		on_mouse_update(t_app *app)
{
	SDL_MouseMotionEvent event;

	event = app->sdl->event.motion;
	if (app->inputs->prev_x == event.x && app->inputs->prev_y == event.y)
		return ;
	if (event.xrel > 0)
		(app->player->direction += app->player->speed * 0.1) > 359 ? app->player->direction = 1 : 0;
	else if (event.xrel < 0)
		(app->player->direction -= app->player->speed * 0.1) < 0 ? app->player->direction = 359 : 0;
	if (event.xrel != 0)
	{
		app->player->x_v = cos(app->player->direction * M_PI_180);
		app->player->y_v = sin(app->player->direction * M_PI_180);
		app->inputs->prev_x = event.x;
		app->inputs->prev_y = event.y;
	}
}

void		start_the_game(t_app *app)
{
	t_time		time;
	Uint64		start;
	Uint64		end;
	float		elapsed;

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	init_time(&time);
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	while (1)
	{
		start = SDL_GetPerformanceCounter();
		if (check_for_quit(app->sdl, app->inputs->keyboard) == 1)
			break ;
		if (app->sdl->event.type == SDL_MOUSEMOTION)
			on_mouse_update(app);
		update_time(&time, app);
		player_debug(app->inputs->keyboard, app->player);
		keyboard_input(app, app->inputs->keyboard, time.frame);
		update_doors(app, time.frame);
		create_field_of_view(app);
		redraw(app->sdl, app->player, &time);
		end = SDL_GetPerformanceCounter();
		elapsed = (float)(end - start) / SDL_GetPerformanceFrequency() * 1000.0;
		SDL_Delay((Uint32)(elapsed - 16.6666));
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
