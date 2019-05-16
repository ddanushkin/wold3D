#include "wolf3d.h"

void		draw_ray_collision(t_sdl *sdl, t_player *player, int x, int y, int r, int g, int b)
{
	SDL_SetRenderDrawColor(sdl->ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(sdl->ren, player->x, player->y, x, y);
	SDL_SetRenderDrawColor(sdl->ren, r, g, b, 255);
	SDL_RenderDrawPoint(sdl->ren, x, y-1);
	SDL_RenderDrawPoint(sdl->ren, x-1, y);
	SDL_RenderDrawPoint(sdl->ren, x, y+1);
	SDL_RenderDrawPoint(sdl->ren, x+1, y);
	SDL_RenderDrawPoint(sdl->ren, x+1, y-1);
	SDL_RenderDrawPoint(sdl->ren, x-1, y+1);
	SDL_RenderDrawPoint(sdl->ren, x+1, y+1);
	SDL_RenderDrawPoint(sdl->ren, x-1, y-1);
}

double get_dist(int p_x, int p_y, double r_x, double r_y)
{
	double d_x;
	double d_y;

	d_x = p_x - r_x;
	d_y = p_y - r_y;
	return sqrt(d_x * d_x + d_y * d_y);
}

double		cast_ray_horz(t_map *map, t_player *player, double angle, t_sdl *sdl)
{
	t_ipoint start;
	t_ipoint step;

	angle = angle * M_PI / 180.0;
	if (sin(angle) < 0)
	{
		start.y = (int)(player->y / 64) * 64 - 1;
		step.y = -64;
	}
	else if(sin(angle) > 0)
	{
		start.y = (int)(player->y / 64) * 64 + 64;
		step.y = 64;
	}
	else
		return 99999;
	step.x = 64 / tan(angle);
	step.x = cos(angle) <= 0 ? -1 * abs(step.x) : abs(step.x);
	start.x = player->x + (start.y - player->y) / (tan(angle));

	while (start.x / 64 < map->cols && start.y / 64 < map->rows)
	{
		if (start.x < 0 || start.y < 0)
			break;
		if (map->nodes[(long)start.y / 64][(long)start.x / 64].collidable)
		{
			draw_ray_collision(sdl, player, start.x, start.y, 255, 0, 0);
			return get_dist(player->x, player->y, start.x, start.y);
		}
		start.x += step.x;
		start.y += step.y;
	}
	return 99999;
}

double	cast_ray_vert(t_map *map, t_player *player, double angle, t_sdl *sdl)
{
	t_ipoint start;
	t_ipoint step;

	angle = angle * M_PI / 180.0;
	if (cos(angle) > 0)
	{
		start.x = (int)(player->x / 64) * 64 + 64;
		step.x = 64;
	}
	else if(cos(angle) < 0)
	{
		start.x = (int)(player->x / 64) * 64 - 1;
		step.x = -64;
	}
	else
		return 99999;
	step.y = 64 * tan(angle);
	step.y = sin(angle) >= 0 ? abs(step.y) : -1 * abs(step.y);
	start.y = player->y + (start.x - player->x) * tan(angle);

	while (start.x / 64 < map->cols && start.y / 64 < map->rows)
	{
		if (start.x < 0 || start.y < 0)
			break;
		if (map->nodes[(long)start.y / 64][(long)start.x / 64].collidable)
		{
			draw_ray_collision(sdl, player, start.x, start.y, 255, 0, 0);
			return get_dist(player->x, player->y, start.x, start.y);
		}
		start.x += step.x;
		start.y += step.y;
	}
	return 99999;
}

void draw_background(t_sdl *sdl)
{
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = sdl->width;
	rect.h = sdl->height / 2;
	SDL_SetRenderDrawColor(sdl->ren, 0, 0, 255, 255);
	SDL_RenderFillRect(sdl->ren, &rect);
	rect.y = sdl->height / 2;
	SDL_SetRenderDrawColor(sdl->ren, 125, 125, 125, 255);
	SDL_RenderFillRect(sdl->ren, &rect);
}

void		cast_rays(t_sdl *sdl, t_map *map, t_player *player)
{
	double		angle;
	int			i;
	double		dist_vert;
	double		dist_horz;
	double		dist;
	double		slice_height;

	draw_background(sdl);
	angle = player->direction - 30.0;
	if (angle < 0)
		angle = 360 + angle;
	i = 0;
	while (i < sdl->width)
	{
		dist_horz = cast_ray_horz(map, player, angle, sdl);
		dist_vert = cast_ray_vert(map, player, angle, sdl);

		if (dist_horz < dist_vert)
			SDL_SetRenderDrawColor(sdl->ren, 195, 0, 255, 255);
		else
			SDL_SetRenderDrawColor(sdl->ren, 0, 195, 255, 255);
		dist = dist_horz < dist_vert ? dist_horz : dist_vert;
		//dist = dist * cos((double)player->direction - angle);
		slice_height = 64.0 / dist * sdl->dist_to_pp;

		SDL_RenderDrawLine(sdl->ren, i, sdl->height/2 - slice_height/2, i, sdl->height/2 + slice_height/2);

		angle = angle + 60.0 / (double)sdl->width;
		if (angle > 360)
			angle = angle - 360;
		i++;
	}
}