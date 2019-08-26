#include "wolf3d.h"

void	start_the_game(t_app *app)
{
	t_fps		fps;

	init_time(app->time, &fps);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	update_time(app->time);
	while (1)
	{
		SDL_PollEvent(&app->sdl->event);
		update_time(app->time);
		if (check_for_quit(&app->sdl->event, app->inputs))
			break ;
		on_mouse_update(app);
		keyboard_input(app, app->time->frame);
		debug_player(app);
		update_doors(app, app->time->frame);
		create_field_of_view(app);
		update_objects(app);
		draw_veiw(app);
		state_change(app);
		animations_update(app);
		redraw(app, app->time->frame);
		get_fps(&fps, app->sdl->renderer);
		SDL_RenderPresent(app->sdl->renderer);
	}
}

int		load_level(t_app *app, int level)
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
		if (map_read(fd, app))
			return (1);
		else
			return (0);
	else
	{
		ft_error("Map path error.");
		return (0);
	}
}

int		keyboard_input2(t_app *app)
{
	const Uint8 *key;

	key = app->inputs->keyboard;
	if (key[SDL_SCANCODE_RETURN])
		return (1);
	return (0);
}

int		display_logo(t_app *app)
{
	SDL_Rect	area;

	area.y = 0;
	area.x = 0;
	area.w = app->sdl->width;
	area.h = app->sdl->height;
	app->inputs->keyboard = SDL_GetKeyboardState(NULL);
	SDL_RenderCopy(app->sdl->renderer, app->sdl->logo, NULL, &area);
	SDL_RenderPresent(app->sdl->renderer);
	while (1)
	{
		SDL_PollEvent(&app->sdl->event);
		if (check_for_quit(&app->sdl->event, app->inputs))
			return (0);
		if (keyboard_input2(app))
			return (1);
	}
}

int		main(void)
{
	t_app	app;

	init(&app);
	player_init(app.sdl, app.player);
	if (display_logo(&app) && load_level(&app, 1))
		start_the_game(&app);
	quit_properly(&app);
	return (0);
}
