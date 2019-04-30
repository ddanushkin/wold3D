#include "wolf3d.h"

static float	line_len(double x_e, double y_e, double x_s, double y_s)
{
	int		len_x;
	int		len_y;
	int		len;

	len_x = abs((int)floor(x_e + 0.5) - (int)floor(x_s));
	len_y = abs((int)floor(y_e + 0.5) - (int)floor(y_s));
	len = MAX(len_x, len_y);
	return (len);
}

void		line_add(t_sdl *sdl, t_ipoint start, t_ipoint end)
{
	t_fpoint	current;
	t_fpoint	delta;
	float		len;

	SDL_SetRenderDrawColor(sdl->ren, 2, 191, 255, 255);
	current.x = start.x + 0.5;
	current.y = start.y + 0.5;
	len = line_len(end.x, end.y, current.x, current.y);
	delta.x = (end.x - start.x) / len;
	delta.y = (end.y - start.y) / len;
	while (len--)
	{
		current.x += delta.x;
		current.y += delta.y;
		SDL_RenderDrawPoint(sdl->ren, current.x, current.y);
	}
}

