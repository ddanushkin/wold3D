#include "wolf3d.h"

static float	line_len(float x_e, float y_e, float x_s, float y_s)
{
	int		len_x;
	int		len_y;
	int		len;

	len_x = abs((int)(x_e - x_s));
	len_y = abs((int)(y_e - y_s));
	len = MAX(len_x, len_y);
	return (len);
}

static void		line_draw(t_sdl *sdl, t_map *map, t_ipoint start, t_ipoint end)
{
	t_fpoint	current;
	t_fpoint	delta;
	float		len;

	current.x = start.x + 0.5;
	current.y = start.y + 0.5;
	len = 1000;
	delta.x = (end.x - start.x) / len;
	delta.y = (end.y - start.y) / len;
	while (len--)
	{
		current.x += delta.x;
		current.y += delta.y;
		if (map->nodes[(int)current.y / MM_SEC_SIZE][(int)current.x / MM_SEC_SIZE].collidable)
			return ;
		SDL_RenderDrawPoint(sdl->ren, (int)current.x, (int)current.y);
	}
}