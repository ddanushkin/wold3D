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

void		start_the_game(t_app *app)
{
	const Uint8	*key;
	t_time		time;
	Uint64		start;
	Uint64		end;
	float		elapsed;

	init_time(&time);
	key = SDL_GetKeyboardState(NULL);
	while (1)
	{
		start = SDL_GetPerformanceCounter();
		if (check_for_quit(app->sdl, key) == 1)
			break ;
		update_time(&time, app);
		player_debug(key, app->player);
		keyboard_input(app, key, time.frame);
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
