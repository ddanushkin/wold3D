#include "wolf3d.h"

void		draw_ray_collision(t_sdl *sdl, t_player *player, int x, int y, int r, int g, int b)
{
	SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(sdl->renderer, player->x, player->y, x, y);
	SDL_SetRenderDrawColor(sdl->renderer, r, g, b, 255);
	SDL_RenderDrawPoint(sdl->renderer, x, y-1);
	SDL_RenderDrawPoint(sdl->renderer, x-1, y);
	SDL_RenderDrawPoint(sdl->renderer, x, y+1);
	SDL_RenderDrawPoint(sdl->renderer, x+1, y);
	SDL_RenderDrawPoint(sdl->renderer, x+1, y-1);
	SDL_RenderDrawPoint(sdl->renderer, x-1, y+1);
	SDL_RenderDrawPoint(sdl->renderer, x+1, y+1);
	SDL_RenderDrawPoint(sdl->renderer, x-1, y-1);
}

float get_dist(int p_x, int p_y, float r_x, float r_y)
{
	float d_x;
	float d_y;

	d_x = p_x - r_x;
	d_y = p_y - r_y;
	return sqrtf(d_x * d_x + d_y * d_y);
}

int is_wall(t_map *map, t_fpoint *pos, t_ray *ray)
{
	if (pos->x < 0 || pos->y < 0 || pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	ray->texture = map->nodes[(int)(pos->y / 64)][(int)(pos->x / 64)].texture;
	return map->nodes[(int)(pos->y / 64)][(int)(pos->x / 64)].collidable;
}

t_ray	*cast_ray_horz(t_map *map, t_player *player, double angle, t_sdl *sdl)
{
	t_fpoint start;
	t_fpoint step;
	t_ray *ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	angle = angle * M_PI / 180.0;
	if (sin(angle) < 0)
	{
		start.y = (int)((player->y / 64) * 64 - 1);
		step.y = -64.0;
	}
	else if(sin(angle) > 0)
	{
		start.y = (int)((player->y / 64) * 64 + 64);
		step.y = 64.0;
	}
	else
	{
		ray->dist = 99999;
		ray->offset = 0;
		ray->texture = NULL;
		return ray;
	}
	step.x = (float)(64.0 / tan(angle));
	step.x = cos(angle) <= 0 ? -fabsf(step.x) : fabsf(step.x);
	start.x = (int)player->x + (start.y - player->y) / (tan(angle));

	while (!is_wall(map, &start, ray))
	{
		start.x += step.x;
		start.y += step.y;
	}
	ray->dist = get_dist(player->x, player->y, start.x, start.y);
	ray->offset = (int)start.x % 64;
	return ray;
}

t_ray	*cast_ray_vert(t_map *map, t_player *player, double angle, t_sdl *sdl)
{
	t_fpoint start;
	t_fpoint step;
	t_ray *ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	angle = angle * M_PI / 180.0;
	if (cos(angle) > 0)
	{
		start.x = (int)((player->x / 64) * 64 + 64);
		step.x = 64.0;
	}
	else if(cos(angle) < 0)
	{
		start.x = (int)((player->x / 64) * 64 - 1);
		step.x = -64.0;
	}
	else
	{
		ray->dist = 99999;
		ray->offset = 0;
		ray->texture = NULL;
		return ray;
	}
	step.y = (float)(64 * tan(angle));
	step.y = sin(angle) >= 0 ? fabsf(step.y) : -fabsf(step.y);
	start.y = (int)(player->y + (start.x - player->x) * tan(angle));

	while (!is_wall(map, &start, ray))
	{
		start.x += step.x;
		start.y += step.y;
	}
	ray->dist = get_dist(player->x, player->y, start.x, start.y);
	ray->offset = (int)start.y % 64;
	return ray;
}

void draw_background(t_sdl *sdl)
{
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = sdl->width;
	rect.h = sdl->height / 2;
	SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(sdl->renderer, &rect);
	rect.y = sdl->height / 2;
	SDL_SetRenderDrawColor(sdl->renderer, 125, 125, 125, 255);
	SDL_RenderFillRect(sdl->renderer, &rect);
}

double	check_angle(double angle)
{
	if (angle < 0)
		angle = 360 + angle;
	else if (angle > 359)
		angle = angle - 360;
	return (angle);
}

void 	color_shade(int dist, SDL_Color *color, double shade_dist)
{
	double factor;

	factor = dist / shade_dist;
	if (factor > 1)
	{
		color->r = 0;
		color->g = 0;
		color->b = 0;
	}
	else
	{
		color->r = color->r * (1 - factor);
		color->g = color->g * (1 - factor);
		color->b = color->b * (1 - factor);
	}
}

void	draw_col(t_sdl * sdl, t_ray *ray, int x, int height, double shade_dist)
{
	int			y;
	int			begin;
	int			end;
	SDL_Color	color;
	double		ratio;
	int			offset;

	ratio = 64.0 / (double)height;
	begin = (int)(sdl->height / 2) - (int)(height / 2);
	end = begin + height;
	y = begin;
	while (y < end)
	{
		offset = (y * sdl->screen->pitch) + (x * 4);
		if (y < sdl->height && offset > 0)
		{
			get_rgb(ray->texture, &color, ray->offset, (y - begin) * ratio);
			color_shade(ray->dist, &color, shade_dist);
			Uint8 *pixel = (Uint8*)sdl->screen->pixels;
			pixel += offset;
			*((Uint32*)pixel) = (Uint32)((color.r << 16) + (color.g << 8) + (color.b << 0));
			//SDL_SetRenderDrawBlendMode(sdl->ren, SDL_BLENDMODE_BLEND);
//			SDL_SetRenderDrawColor(sdl->ren, (int)(color.r * ray->dist) % 255, (int)(color.g * ray->dist) % 255, (int)(color.b * ray->dist) % 255, 255);
			//SDL_SetRenderDrawColor(sdl->ren, color.r, color.g, ç, 255);
			//SDL_RenderDrawPoint(sdl->ren, x, y);
		}
		y++;
	}
}

void		cast_rays(t_sdl *sdl, t_map *map, t_player *player)
{
	float		angle;
	int			x;
	t_ray		*ray_vert;
	t_ray		*ray_horz;
	t_ray		*hit_ray;
	double		dist;
	int			slice_height;

	//draw_background(sdl);
	angle = player->direction - 30.0;
	angle = check_angle(angle);
	x = 0;
	while (x < sdl->width)
	{
		ray_horz = cast_ray_horz(map, player, angle, sdl);
		ray_vert = cast_ray_vert(map, player, angle, sdl);
		hit_ray = ray_horz->dist < ray_vert->dist ? ray_horz : ray_vert;
		dist = hit_ray->dist * cos((player->direction - angle) * M_PI / 180.0);
		if (dist < player->shade_dist)
		{
			slice_height = 64 / dist * sdl->dist_to_pp;
			draw_col(sdl, hit_ray, x, slice_height, player->shade_dist);
		}
		angle = angle + 60.0 / sdl->width;
		angle = check_angle(angle);
		x++;
	}
}