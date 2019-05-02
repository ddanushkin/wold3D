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

static void		line_draw(t_sdl *sdl, t_map *map, t_ipoint start, t_ipoint end)
{
	t_fpoint	current;
	t_fpoint	delta;
	float		len;

	current.x = start.x + 0.5;
	current.y = start.y + 0.5;
	len = line_len(end.x, end.y, current.x, current.y);
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

void			cast_ray(t_sdl *sdl, t_map *map, t_player *player, int a)
{
	t_ipoint s;
	t_ipoint e;

	s.x = player->x;
	s.y = player->y;

	end_point(a, s, &e, 1000);
	line_draw(sdl, map, s, e);
}