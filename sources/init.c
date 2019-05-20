#include "wolf3d.h"

void		init(t_sdl *sdl)
{
	sdl->width = 640;
	sdl->height = 480;
	sdl->fov = 3.14159 / 3.0; //0.66;
	sdl->dist_to_pp = sdl->width / (tan(sdl->fov / 2.0) * 2.0);
	sdl->draw_dist = 900;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL initialization error");
	sdl->window = SDL_CreateWindow("SDL2 Example", 0, 0, sdl->width, sdl->height, 0);
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	sdl->ceiling = -5;
}
