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
		collision->dist = sqrt(ft_powi((player->x - start.x), 2) * 2);
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
			collision->dist = sqrt(ft_powi((player->x - start.x), 2) * 2);
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
		collision->dist = sqrt(ft_powi((player->x - start.x), 2) * 2);
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
			collision->dist = sqrt(ft_powi((player->x - start.x), 2) * 2);
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
	int					slice_height;

	inc = (60.0 / sdl->width * 100) / 100.0;
	i = player->direction - 30;
	if (i <= 0)
		i = 360.0 + i;
	j = 0;
	while(j < sdl->width)
	{
		coll_vert = cast_ray_v(map, player, i);
		coll_horz = cast_ray_h(map, player, i);
		if (j == sdl->width / 2)
		{
			if (coll_horz->dist < coll_vert->dist)
				printf("%d\n", coll_horz->dist);
			else
				printf("%d\n", coll_vert->dist);
		}
		if (coll_horz->dist < coll_vert->dist)
		{
			draw_ray_collision(sdl, player, *coll_horz, 0, 0, 255);
			//a_rad = (player->direction - i) * M_PI_180;
			//dist = coll_horz->dist * cos(a_rad);
		}
		else
		{
			draw_ray_collision(sdl, player, *coll_vert, 0, 255, 0);
			//a_rad = (player->direction - i) * M_PI_180;
			//dist = coll_vert->dist * sin(a_rad);
		}
		double dx = coll_horz->dist * cos(i);
		double dy = coll_vert->dist * sin(i);
		dist = dx * cos(player->direction) + dy * sin(player->direction);
/*
		if (dist != 0)
		{
			//slice_height = sdl->height * sdl->height / dist;
			//int wallHeight = sdl->height * sdl->height / dist;
			//int bottom = sdl->height / 2 * (1 + 1 / dist);
			//slice_height = (64 / dist) * (sdl->dist_to_pp / 64) * 8;
			slice_height = (64 / dist);
			SDL_SetRenderDrawColor(sdl->ren, 195, 0, 255, 255);
			SDL_RenderDrawLine(sdl->ren, j, sdl->height/2 - slice_height/2, j, sdl->height/2 + slice_height/2);
			//SDL_RenderDrawLine(sdl->ren, j, bottom, j, wallHeight);
		}
		else
		{
			//slice_height = sdl->height * sdl->height / dist;
			//int wallHeight = sdl->height * sdl->height / dist;
			//int bottom = sdl->height / 2 * (1 + 1 / dist);
			slice_height = (sdl->dist_to_pp);
			//slice_height = (64 / dist);
			SDL_SetRenderDrawColor(sdl->ren, 195, 0, 255, 255);
			SDL_RenderDrawLine(sdl->ren, j, sdl->height/2 - slice_height/2, j, sdl->height/2 + slice_height/2);
			//SDL_RenderDrawLine(sdl->ren, j, bottom, j, wallHeight);
		}*/
		i += inc;
		if (i > 360)
			i = 360.0 - i;
		j++;
	}
	SDL_SetRenderDrawColor(sdl->ren, 255, 50, 50, 255);
	a_rad = (player->direction) * M_PI_180;
	SDL_RenderDrawLine(sdl->ren, player->x, player->y, player->x + 15 * cos(a_rad), player->y + 15 * sin(-a_rad));
}