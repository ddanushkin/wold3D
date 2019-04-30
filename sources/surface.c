#include "wolf3d.h"

void get_rgb(SDL_Surface *surface, SDL_Color *c, int x, int y)
{
	int bpp;
	Uint8 *data;

	bpp = surface->format->BytesPerPixel;
	data = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	SDL_GetRGB(*data, surface->format, &c->r, &c->g, &c->b);
}