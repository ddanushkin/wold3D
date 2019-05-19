#include "wolf3d.h"

void get_rgb(SDL_Surface *surface, SDL_Color *c, int x, int y)
{
	Uint8	*data;

	data = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
	SDL_GetRGB(*data, surface->format, &c->r, &c->g, &c->b);
}