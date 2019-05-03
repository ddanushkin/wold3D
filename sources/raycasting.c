#include "wolf3d.h"

void end_point(float angle, t_fpoint s, t_fpoint *e, int length)
{
	angle = angle * (M_PI / 180.0);
	e->x = s.x + (length * cos(angle));
	e->y = s.y + (length * sin(angle));
}

static void	cast_ray(t_sdl *sdl, t_map *map, t_player *player, int a)
{
	t_fpoint s;
	t_fpoint e;

	s.x = player->x;
	s.y = player->y;

	end_point(a, s, &e, 1000);
	SDL_RenderDrawLine(sdl->ren, s.x, s.y, e.x, e.y);
}

void		cast_rays(t_sdl *sdl, t_map *map, t_player *player, int fov)
{
	float		i;
	float		inc;

	inc = (float)(fov)/sdl->width;
	i = 0;
	SDL_SetRenderDrawColor(sdl->ren, 255, 0, 0, 255);
	cast_ray(sdl, map, player, player->direction);
	while(i < 30)
	{
		cast_ray(sdl, map, player, player->direction + i);
		cast_ray(sdl, map, player, player->direction - i);
		i = i + inc;
	}
}

