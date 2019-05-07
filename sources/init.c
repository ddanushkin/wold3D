#include "wolf3d.h"

void		init(t_sdl *sdl)
{
	sdl->width = 1280;
	sdl->height = 720;
	sdl->fov = 3.14159 / 3.0;
	sdl->dist_to_pp = (sdl->width / 2.0) / tan(sdl->fov / 2.0);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL initialization error");
	if (SDL_CreateWindowAndRenderer(sdl->width, sdl->height, 0, &sdl->win, &sdl->ren) == -1)
		ft_error("SDL window creation error");
}
