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

void		update(t_sdl *sdl, t_map *map, t_player *player)
{
	const Uint8	*state;
	static Uint32 next_time;

	while (1)
	{
		SDL_SetRenderDrawColor(sdl->ren, 0, 0, 0, 0);
		SDL_RenderClear(sdl->ren);
		if (SDL_PollEvent(&sdl->event) && sdl->event.type == SDL_QUIT)
			break;
		state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
			break;
		minimap_draw(map, sdl, player);
		player_move(map, state, player);
		SDL_RenderPresent(sdl->ren);
		SDL_Delay(time_left(next_time));
		next_time += TICK_INTERVAL;
	}
}

int		main(void)
{
	t_app	app;
	int		fd;

	init(&app.sdl);
	player_init(&app.player);
	if ((fd = open("../levels/1.wolf3d", O_RDONLY)) != -1)
	{
		map_read(fd, &app.map, &app.player);
		update(&app.sdl, &app.map, &app.player);
	}
	else
		ft_error("Map path error.");
	quit_properly(&app);
	return (0);
}
