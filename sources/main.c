#include "wolf3d.h"

Uint32 time_left(Uint32 next_time)
{
	Uint32 now;

	now = SDL_GetTicks();
	if(next_time <= now)
		return 0;
	else
		return next_time - now;
}

void		update(t_app *app)
{
	const Uint8	*button;
	static Uint32 next_time;

	while (1)
	{
		//SDL_FillRect(sdl->screen, NULL, 0x000000);
		if (SDL_PollEvent(&app->sdl->event) && app->sdl->event.type == SDL_QUIT)
			break;
		button = SDL_GetKeyboardState(NULL);
		if (button[SDL_SCANCODE_ESCAPE])
			break;
		//draw_minimap(map, sdl, player);
		cast_rays(app);
		player_move(app->map, button, app->player);
		SDL_UpdateWindowSurface(app->sdl->window);
		SDL_Delay(time_left(next_time));
		next_time += TICK_INTERVAL;
	}
}

int		main(void)
{
	t_app	app;
	int		fd;

	init(&app);
	player_init(app.player);
	if ((fd = open("../levels/1.wolf3d", O_RDONLY)) != -1)
	{
		map_read(fd, app.map, app.player);
		update(&app);
	}
	else
		ft_error("Map path error.");
	quit_properly(&app);
	return (0);
}
