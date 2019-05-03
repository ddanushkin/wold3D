#include "wolf3d.h"

void end_point(double a, int x1, int y1, t_ipoint *e, int length)
{
	a = a * (M_PI / 180.0);
	e->x = x1 + length * cos(a);
	e->y = y1 + length * sin(a);
}

static void	cast_ray(t_sdl *sdl, t_map *map, t_player *player, double a)
{
	t_ipoint intersaction;
	t_ipoint e;
	t_ipoint s;

	end_point(a, player->x, player->y, &e, 10);
	a = a * (M_PI / 180.0);
	if (a == 0 || a == 180)
		return ;
	if (a > 0 && a < 180)
		intersaction.y = (player->y / 64) * (64) + 64;
	else
		intersaction.y = (player->y / 64) * (64) - 1;

	intersaction.x = player->x + (player->y - intersaction.y) / tan(a);
	SDL_SetRenderDrawColor(sdl->ren, 0, 255, 0, 255);
	SDL_RenderDrawPoint(sdl->ren, intersaction.x, intersaction.y);
	printf("[%d][%d]\n", intersaction.y/64, intersaction.x/64);
	SDL_RenderDrawLine(sdl->ren, player->x, player->y, e.x, e.y);
}
void		cast_rays(t_sdl *sdl, t_map *map, t_player *player, int fov)
{
	float		i;
	float		inc;

	inc = (float)(fov)/sdl->width;
	i = 0;
	SDL_SetRenderDrawColor(sdl->ren, 255, 0, 0, 255);
	cast_ray(sdl, map, player, player->direction);
//	while(i < 30)
//	{
//		cast_ray(sdl, map, player, player->direction + i);
//		cast_ray(sdl, map, player, player->direction - i);
//		i = i + inc;
//		printf("%f\n", i);
//	}
}

