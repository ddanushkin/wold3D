#include "wolf3d.h"

void		init(t_sdl *sdl, t_player *player)
{
	sdl->width = 640;
	sdl->height = 480;

	player->x = sdl->width / 2;
	player->y = sdl->height / 2;

	//float fFOV = 3.14159 / 4.0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL initialization error");
	if (SDL_CreateWindowAndRenderer(sdl->width, sdl->height, 0, &sdl->win, &sdl->ren) == -1)
		ft_error("SDL window creation error");
}
