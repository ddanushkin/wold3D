#include "wolf3d.h"

float power(float base, int exp)
{
	if(exp < 0) {
		if(base == 0)
			return -0;
		return 1 / (base * power(base, (-exp) - 1));
	}
	if(exp == 0)
		return 1;
	if(exp == 1)
		return base;
	return base * power(base, exp - 1);
}

int fact(int n)
{
	return n <= 0 ? 1 : n * fact(n-1);
}

float sine(int deg)
{
	deg %= 360;
	float rad = deg * M_PI / 180;
	float sin = 0;

	int i;
	for(i = 0; i < 7; i++)
		sin += power(-1, i) * power(rad, 2 * i + 1) / fact(2 * i + 1);
	return sin;
}

float cosine(int deg)
{
	deg %= 360;
	float rad = deg * M_PI / 180;
	float cos = 0;

	int i;
	for(i = 0; i < 7; i++)
		cos += power(-1, i) * power(rad, 2 * i) / fact(2 * i);
	return cos;
}

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
	t_ipoint			start;
	t_ipoint			step;
	t_collision_point	*collision;
	double				a_rad;

	collision = (t_collision_point *)malloc(sizeof(t_collision_point));
	a_rad = (a - 90.0) * (M_PI / 180.0);
	if (a == 90 || a == 270)
		return(NULL);
	if (a > 90 && a < 270)
	{
		start.x = (player->x / 64) * 64 - 1;
		start.y = player->y + (start.x - player->x) / tan(a_rad);
		step.x = -64;
		step.y = 64 / tan(-a_rad);
	}
	else
	{
		start.x = (player->x / 64) * 64 + 64;
		start.y = player->y + (start.x - player->x) / tan(a_rad);
		step.x = 64;
		step.y = 64 / tan(a_rad);
	}
	if (start.y < 0 || start.x < 0 || (int)(start.y / 64) >= map->rows || (int)(start.x / 64) >= map->cols)
		return(NULL);
	if (map->nodes[(int)(start.y / 64)][(int)(start.x / 64)].collidable)
	{
		collision->x = start.x;
		collision->y = start.y;
		collision->dist = (abs(player->x - start.x) * cosine(a_rad)) + (abs(player->y - start.y) * sine(a_rad));
		printf("vert distance - %d\n", collision->dist);
		return(collision);
	}
	while(start.x >= 0 && start.x < map->cols * 64 && start.y >= 0 && start.y < map->rows * 64)
	{
		start.x = start.x + step.x;
		start.y = start.y + step.y;
		if (start.y < 0 || start.x < 0 || (int)(start.y / 64) >= map->rows || (int)(start.x / 64) >= map->cols)
			return(NULL);
		if (map->nodes[(int)(start.y / 64)][(int)(start.x / 64)].collidable)
		{
			collision->x = start.x;
			collision->y = start.y;
			collision->dist = (abs(player->x - start.x) * cosine(a_rad)) + (abs(player->y - start.y) * sine(a_rad));
			printf("vert distance - %d\n", collision->dist);
			return(collision);
		}
	}
	return(NULL);
}

static t_collision_point	*cast_ray_h(t_map *map, t_player *player, double a)
{
	t_ipoint			start;
	t_ipoint			step;
	t_collision_point	*collision;
	double				a_rad;

	collision = (t_collision_point *)malloc(sizeof(t_collision_point));
	a_rad = a * (M_PI / 180.0);
	if (a == 0 || a == 180)
		return(NULL);
	if (a > 0 && a < 180)
	{
		start.y = (player->y / 64) * 64 - 1;
		start.x = player->x + (player->y - start.y) / tan(a_rad);
		step.y = -64;
		step.x = 64 / tan(a_rad);
	}
	else
	{
		start.y = (player->y / 64) * 64 + 64;
		start.x = player->x + (player->y - start.y) / tan(a_rad);
		step.y = 64;
		step.x = 64 / tan(-a_rad);
	}
	if (start.y < 0 || start.x < 0 || (int)(start.y / 64) >= map->rows || (int)(start.x / 64) >= map->cols)
		return(NULL);
	if (map->nodes[(int)(start.y / 64)][(int)(start.x / 64)].collidable)
	{
		collision->x = start.x;
		collision->y = start.y;
		collision->dist = (abs(player->x - start.x) * cosine(a_rad)) + (abs(player->y - start.y) * sine(a_rad));
		printf("horz distance - %d\n", collision->dist);
		return(collision);
	}
	while(start.x >= 0 && start.x < map->cols * 64 && start.y >= 0 && start.y < map->rows * 64)
	{
		start.x = start.x + step.x;
		start.y = start.y + step.y;
		if (map->nodes[(int)(start.y / 64)][(int)(start.x / 64)].collidable)
		{
			collision->x = start.x;
			collision->y = start.y;
			collision->dist = (abs(player->x - start.x) * cosine(a_rad)) + (abs(player->y - start.y) * sine(a_rad));
			printf("horz distance - %d\n", collision->dist);
			return(collision);
		}
	}
	return(NULL);
}

void		cast_rays(t_sdl *sdl, t_map *map, t_player *player, int fov)
{
	float		i;
	double		inc;
	t_collision_point	*coll_horz;
	t_collision_point	*coll_vert;

	double a_rad;

	inc = (double)fov/sdl->width;
	i = player->direction - 30;
	while(i < player->direction + 30)
	{
		coll_horz = cast_ray_h(map, player, i);
		coll_vert = cast_ray_v(map, player, i);
		if (coll_horz != NULL && coll_vert != NULL)
		{
			if (coll_horz->dist <= coll_vert->dist)
			{
				draw_ray_collision(sdl, player, *coll_horz, 0, 0, 255);
				free(coll_horz);
			}
			else
			{
				draw_ray_collision(sdl, player, *coll_vert, 0, 255, 0);
				free(coll_vert);
			}
		}
		else
		{
			if (coll_horz != NULL)
			{
				draw_ray_collision(sdl, player, *coll_horz, 0, 0, 255);
				free(coll_horz);
			}
			if (coll_vert != NULL)
			{
				draw_ray_collision(sdl, player, *coll_vert, 0, 255, 0);
				free(coll_vert);
			}
		}
		i = i + inc;
	}

	SDL_SetRenderDrawColor(sdl->ren, 255, 50, 50, 255);
	a_rad = (player->direction) * (M_PI / 180.0);
	SDL_RenderDrawLine(sdl->ren, player->x, player->y, player->x + 15 * cos(a_rad), player->y + 15 * sin(-a_rad));
}

