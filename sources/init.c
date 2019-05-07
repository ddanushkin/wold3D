#include "wolf3d.h"

void		init(t_sdl *sdl)
{
	sdl->width = 7 * MM_SEC_SIZE;
	sdl->height = 13 * MM_SEC_SIZE;

	//float fFOV = 3.14159 / 4.0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL initialization error");
	if (SDL_CreateWindowAndRenderer(sdl->width, sdl->height, 0, &sdl->win, &sdl->ren) == -1)
		ft_error("SDL window creation error");
}
