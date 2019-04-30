#include "wolf3d.h"

void		update(t_sdl *sdl, t_map *map, t_player *player)
{
	const Uint8	*state;

	while (1)
	{
		SDL_SetRenderDrawColor(sdl->ren, 0, 0, 0, 0);
		SDL_RenderClear(sdl->ren);
		if (SDL_PollEvent(&sdl->event) && sdl->event.type == SDL_QUIT)
			break;
		state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
			break;
		if (state[SDL_SCANCODE_LEFT])
			player->x++;
		if (state[SDL_SCANCODE_RIGHT])
			player->x--;
		if (state[SDL_SCANCODE_UP])
			player->y++;
		if (state[SDL_SCANCODE_DOWN])
			player->y--;
		draw_minimap(map, sdl, player);
		SDL_RenderPresent(sdl->ren);
	}
}

int		main(void)
{
	t_app		app;
	int fd;

	init(&app.sdl, &app.player);

	if ((fd = open("../levels/1.wolf3d", O_RDONLY)) != -1)
	{
		read_map(fd, &app.map, &app.player);
		update(&app.sdl, &app.map, &app.player);
	}
	else
		ft_error("Map path error.");
	quit_properly(&app);
	return (0);
}
