#include "wolf3d.h"

void	get_color(SDL_Surface *surface, SDL_Color *c, int x, int y)
{
	Uint8	*data;

	data = surface->pixels + y * surface->pitch;
	data += x * surface->format->BytesPerPixel;
	SDL_GetRGB(*data, surface->format, &c->r, &c->g, &c->b);
}

void	shade_color(double dist, SDL_Color *color, double draw_dist)
{
	double	factor;

	factor = dist / draw_dist;
//	else if (factor < 0.3)
//	{
//		color->r = color->r / factor;
//		color->g = color->g / factor;
//		color->b = color->b / factor;
//	}
	if (factor > 1)
	{
		color->r = 0;
		color->g = 0;
		color->b = 0;
	}
	if (factor > 0.5)
	{
		color->r = color->r * (1 - factor);
		color->g = color->g * (1 - factor);
		color->b = color->b * (1 - factor);
	}
}

void	set_pixel(t_sdl *sdl, int x, int y, SDL_Color *col)
{
	int		offset;

	offset = y * sdl->width + x;
	sdl->pixels[offset] = (Uint32)((col->r << 16) + (col->g << 8) + (col->b));
}
