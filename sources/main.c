#include "wolf3d.h"

void		start_the_game(t_app *app)
{
	t_fps	fps;

	init_time(app->time, &fps);
	printf("SetRelativeMouseMode -> %d\n", SDL_SetRelativeMouseMode(SDL_TRUE));
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	while (1)
	{
		update_time(app->time);
		SDL_PollEvent(&app->sdl->event);
		if (check_for_quit(&app->sdl->event, app->inputs) == 1)
			break ;
		on_mouse_update(app->inputs, app->player, app->time->delta);
		keyboard_input(app, app->time->frame);
		update_doors(app, app->time->frame);
		reset_objects(app->map);
		create_field_of_view(app);
		update_objects(app);
		redraw(app->sdl, app->player, app->time->frame);
		get_fps(&fps, app->sdl->renderer);
		SDL_RenderPresent(app->sdl->renderer);
	}
}

void		load_level(t_player *player, t_map *map, int level)
{
	int		fd;
	char	*level_char;
	char	level_path[50];

	ft_strdel(&player->cur_level);
	level_char = ft_itoa(level);
	player->cur_level = level_char;
	ft_strcpy(level_path, "../levels/");
	ft_strcat(level_path, level_char);
	ft_strcat(level_path, ".wolf3d");
	if ((fd = open(level_path, O_RDONLY)) != -1)
		map_read(fd, map, player);
	else
		ft_error("Map path error.");
}

int			main(void)
{
	t_app	app;

	init(&app);
	player_init(app.sdl, app.player);
	load_level(app.player, app.map, 1);
	start_the_game(&app);
	quit_properly(&app);
	return (0);
}
