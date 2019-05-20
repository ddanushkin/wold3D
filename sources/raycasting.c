#include "wolf3d.h"

void	draw_ray_collision(t_sdl *sdl, t_player *player, int x, int y, int r, int g, int b)
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

int is_wall_h(t_map *map, t_fpoint *pos, t_ray *ray, double angle)
{
	if (pos->x < 0 || pos->y < 0 || pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	if (sin(angle) > 0)
		ray->texture = map->nodes[(int)(pos->y / 64)][(int)(pos->x / 64)].texture_w;
	else
		ray->texture = map->nodes[(int)(pos->y / 64)][(int)(pos->x / 64)].texture_e;
	ray->x = (int)pos->x;
	ray->y = (int)pos->y;
	return map->nodes[(int)(pos->y / 64)][(int)(pos->x / 64)].collidable;
}

int is_wall_v(t_map *map, t_fpoint *pos, t_ray *ray, double angle)
{
	if (pos->x < 0 || pos->y < 0 || pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	if (cos(angle) < 0)
		ray->texture = map->nodes[(int)(pos->y / 64)][(int)(pos->x / 64)].texture_n;
	else
		ray->texture = map->nodes[(int)(pos->y / 64)][(int)(pos->x / 64)].texture_s;
	ray->x = (int)pos->x;
	ray->y = (int)pos->y;
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
		start.y = (int)(player->y / 64) * 64.0 - 1.0;
		step.y = -64;
	}
	else if(sin(angle) > 0)
	{
		start.y = (int)(player->y / 64) * 64.0 + 64.0;
		step.y = 64;
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

	while (!is_wall_h(map, &start, ray, angle))
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
		start.x = (int)(player->x / 64) * 64.0 + 64.0;
		step.x = 64.0;
	}
	else if(cos(angle) < 0)
	{
		start.x = (int)(player->x / 64) * 64.0 - 1.0;
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

	while (!is_wall_v(map, &start, ray, angle))
	{
		start.x += step.x;
		start.y += step.y;
	}
	ray->dist = get_dist(player->x, player->y, start.x, start.y);
	ray->offset = (int)start.y % 64;
	return ray;
}

double	check_angle(double angle)
{
	if (angle < 0)
		angle = 360 + angle;
	else if (angle > 359)
		angle = angle - 360;
	return (angle);
}

void 	shade_color(int dist, SDL_Color *color, double shade_dist)
{
	double factor;

	factor = dist / (shade_dist - 60.0);
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

void	get_color(SDL_Surface *surface, SDL_Color *c, int x, int y)
{
	Uint8	*data;

	data = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
	SDL_GetRGB(*data, surface->format, &c->r, &c->g, &c->b);
}

void	set_pixel(t_sdl *sdl, int x, int y, SDL_Color *color)
{
	int			offset;

	offset = (y * sdl->screen->pitch) + (x * 4);
	Uint8 *pixel = (Uint8*)sdl->screen->pixels;
	pixel += offset;
	*((Uint32*)pixel) = (Uint32)((color->r << 16) + (color->g << 8) + (color->b));
}

void	draw_col(t_sdl * sdl, t_ray *ray, int x, int height)
{
	int			y;
	int			begin;
	int			end;
	double		ratio;

	int j;
	double		vert_dist;

	SDL_Color	color;

	ratio = 64.0 / height;
	begin = (int)(sdl->height / 2) - (int)(height / 2);
	if ((end = begin + height) > sdl->height)
		end = sdl->height;
	if ((y = begin) < 0)
		y = 0;

	if (y > 0)
	{
		j = 0;
		while (j < y)
		{
			color.r = 31;
			color.g = 44;
			color.b = 61;
			vert_dist = (sdl->height / (2.0 * j - sdl->height)) * -sdl->ceiling;
			shade_color(vert_dist, &color, 2);
			set_pixel(sdl, x, j, &color);
			j++;
		}
	}

	if (end < sdl->height)
	{
		j = end;
		while (j < sdl->height)
		{
			color.r = 21;
			color.g = 34;
			color.b = 51;
			vert_dist = (sdl->height / (2.0 * j - sdl->height))  * sdl->ceiling;
			shade_color(vert_dist, &color, 2);
			set_pixel(sdl, x, j, &color);
			j++;
		}
	}

	while (y < end)
	{
		get_color(ray->texture, &color, ray->offset, (y - begin) * ratio);
		shade_color(ray->dist, &color, sdl->draw_dist);
		set_pixel(sdl, x, y, &color);
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
	float		dist;
	int			slice_height;

	angle = player->direction - 30.0;
	//angle = check_angle(angle);
	x = 0;
	while (x < sdl->width)
	{
		ray_horz = cast_ray_horz(map, player, angle, sdl);
		ray_vert = cast_ray_vert(map, player, angle, sdl);
		hit_ray = ray_horz->dist < ray_vert->dist ? ray_horz : ray_vert;
		dist = hit_ray->dist * cos((angle - player->direction) * M_PI / 180.0);
		if (dist < player->shade_dist)
		{
			slice_height = 64 / dist * sdl->dist_to_pp;
			draw_col(sdl, hit_ray, x, slice_height);
		}
		free(ray_horz);
		free(ray_vert);
		angle = angle + (60.0 / sdl->width);
		//angle = check_angle(angle);
		x++;
	}
}