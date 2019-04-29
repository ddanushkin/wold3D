#include "wolf3d.h"

void		init(t_sdl *sdl, t_player *player)
{
	sdl->width = 640;
	sdl->height = 480;

	player->x = sdl->width / 2;
	player->y = sdl->height / 2;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(sdl->width, sdl->height, 0, &sdl->win, &sdl->ren);
}