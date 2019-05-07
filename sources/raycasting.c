#include "wolf3d.h"

void		draw_ray_collision(t_sdl *sdl, t_player *player, t_collision_point start, int r, int g, int b)
{
	SDL_SetRenderDrawColor(sdl->ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(sdl->ren, player->x, player->y, start.x, start.y);
	SDL_SetRenderDrawColor(sdl->ren, r, g, b, 255);
	SDL_RenderDrawPoint(sdl->ren, start.x, start.y-1);
	SDL_RenderDrawPoint(sdl->ren, start.x-1, start.y);
	SDL_RenderDrawPoint(sdl->ren, start.x, start.y+1);
	SDL_RenderDrawPoint(sdl->ren, start.x+1, start.y);
	SDL_RenderDrawPoint(sdl->ren, start.x+1, start.y-1);
	SDL_RenderDrawPoint(sdl->ren, start.x-1, start.y+1);
	SDL_RenderDrawPoint(sdl->ren, start.x+1, start.y+1);
	SDL_RenderDrawPoint(sdl->ren, start.x-1, start.y-1);
}

static t_collision_point	*cast_ray_v(t_map *map, t_player *player, double a)
{
	t_fpoint			start;
	t_fpoint			step;
	t_collision_point	*collision;
	double				a_rad;

	collision = (t_collision_point *)malloc(sizeof(t_collision_point));
	a_rad = (a - 90.0) * (M_PI / 180.0);
	if (a == 90 || a == 270)
	{
		collision->dist = 9999;
		return(collision);
	}
	if (a > 90 && a < 270)
	{
		start.x = (player->x / 64) * 64 - 1;
		step.x = -64;
		step.y = 64 / tan(-a_rad);
	}
	else
	{
		start.x = (player->x / 64) * 64 + 64;
		step.x = 64;
		step.y = 64 / tan(a_rad);
	}
	start.y = player->y + (start.x - player->x) / tan(a_rad);
	if (start.y < 0 || start.x < 0 || (long)(start.y / 64) >= map->rows || (long)(start.x / 64) >= map->cols)
	{
		collision->dist = 9999;
		return(collision);
	}
	if (map->nodes[(long)(start.y / 64)][(long)(start.x / 64)].collidable)
	{
		collision->x = start.x;
		collision->y = start.y;
		collision->dist = sqrt(abs(player->x - start.x) * abs(player->x - start.x) + abs(player->x - start.x) * abs(player->x - start.x));
		return(collision);
	}
	while(start.x >= 0 && start.x < map->cols * 64 && start.y >= 0 && start.y < map->rows * 64)
	{
		start.x = start.x + step.x;
		start.y = start.y + step.y;
		if (start.y < 0 || start.x < 0 || (int)(start.y / 64) >= map->rows || (int)(start.x / 64) >= map->cols)
		{
			collision->dist = 9999;
			return(collision);
		}
		if (map->nodes[(long)(start.y / 64)][(long)(start.x / 64)].collidable)
		{
			collision->x = start.x;
			collision->y = start.y;
			collision->dist = sqrt(abs(player->x - start.x) * abs(player->x - start.x) + abs(player->x - start.x) * abs(player->x - start.x));
			return(collision);
		}
	}
	collision->dist = 9999;
	return(collision);
}

static t_collision_point	*cast_ray_h(t_map *map, t_player *player, double a)
{
	t_fpoint			start;
	t_fpoint			step;
	t_collision_point	*collision;
	double				a_rad;

	collision = (t_collision_point *)malloc(sizeof(t_collision_point));
	a_rad = a * (M_PI / 180.0);
	if (a == 0 || a == 180)
	{
		collision->dist = 9999;
		return(collision);
	}
	if (a > 0 && a < 180)
	{
		start.y = (player->y / 64) * 64 - 1;
		step.y = -64;
		step.x = -step.y / tan(a_rad);
	}
	else
	{
		start.y = (player->y / 64) * 64 + 64;
		step.y = 64;
		step.x = step.y / tan(-a_rad);
	}
	start.x = player->x + (player->y - start.y) / tan(a_rad);
	if (start.y < 0 || start.x < 0 || (long)(start.y / 64) >= map->rows || (long)(start.x / 64) >= map->cols)
	{
		collision->dist = 9999;
		return(collision);
	}
	if (map->nodes[(long)start.y / 64][(long)start.x / 64].collidable)
	{
		collision->x = start.x;
		collision->y = start.y;
		collision->dist = sqrt(abs(player->x - start.x) * abs(player->x - start.x) + abs(player->x - start.x) * abs(player->x - start.x));
		return(collision);
	}
	while(start.x >= 0 && start.x < map->cols * 64 && start.y >= 0 && start.y < map->rows * 64)
	{
		start.x = start.x + step.x;
		start.y = start.y + step.y;
		if (start.y < 0 || start.x < 0 || (long)(start.y / 64) >= map->rows || (long)(start.x / 64) >= map->cols)
		{
			collision->dist = 9999;
			return(collision);
		}
		if (map->nodes[(long)(start.y / 64)][(long)(start.x / 64)].collidable)
		{
			collision->x = start.x;
			collision->y = start.y;
			collision->dist = sqrt(abs(player->x - start.x) * abs(player->x - start.x) + abs(player->x - start.x) * abs(player->x - start.x));
			return(collision);
		}
	}
	collision->dist = 9999;
	return(collision);
}

void		cast_rays(t_sdl *sdl, t_map *map, t_player *player, int fov)
{
	float				i;
	int					j;
	float				inc;
	t_collision_point	*coll_horz;
	t_collision_point	*coll_vert;
	double				a_rad;
	int					dist;
	int					slice_hieght;

	inc = floor(60.0/sdl->width*100)/100.0;
	i = (float)player->direction - 30.0;
	if (i <= 0)
		i = 360.0 + i;
	j = 0;
	while(j < sdl->width)
	{
		coll_horz = cast_ray_h(map, player, i);
		coll_vert = cast_ray_v(map, player, i);
		if (coll_horz->dist < coll_vert->dist)
		{
			//draw_ray_collision(sdl, player, *coll_horz, 0, 0, 255);
			a_rad = (player->direction - i) * (M_PI / 180.0);
			dist = coll_horz->dist * (cos(a_rad));
		}
		else
		{
			//draw_ray_collision(sdl, player, *coll_vert, 0, 255, 0);
			a_rad = (player->direction - i) * (M_PI / 180.0);
			dist = coll_vert->dist * (cos(a_rad));
		}
		if (dist != 0)
		{
			slice_hieght = 64 / dist * sdl->dist_to_pp;
			SDL_SetRenderDrawColor(sdl->ren, 195, 0, 255, 255);
			SDL_RenderDrawLine(sdl->ren, j, sdl->height/2 - slice_hieght/2, j, sdl->height/2 + slice_hieght/2);
		}
		i += inc;
		if (i >= 360)
			i = 360.0 - i;
		j++;
	}
	SDL_SetRenderDrawColor(sdl->ren, 255, 50, 50, 255);
	a_rad = (player->direction) * (M_PI / 180.0);
	SDL_RenderDrawLine(sdl->ren, player->x, player->y, player->x + 15 * cos(a_rad), player->y + 15 * sin(-a_rad));
}

