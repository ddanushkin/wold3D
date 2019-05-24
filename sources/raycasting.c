/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:21:40 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/23 16:10:25 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_ray_collision(t_sdl *sdl, t_player *player, int x, int y, int r, int g, int b)
{
	SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(sdl->renderer, player->x, player->y, x, y);
	SDL_SetRenderDrawColor(sdl->renderer, r, g, b, 255);
	SDL_RenderDrawPoint(sdl->renderer, x, y - 1);
	SDL_RenderDrawPoint(sdl->renderer, x - 1, y);
	SDL_RenderDrawPoint(sdl->renderer, x, y + 1);
	SDL_RenderDrawPoint(sdl->renderer, x + 1, y);
	SDL_RenderDrawPoint(sdl->renderer, x + 1, y - 1);
	SDL_RenderDrawPoint(sdl->renderer, x - 1, y + 1);
	SDL_RenderDrawPoint(sdl->renderer, x + 1, y + 1);
	SDL_RenderDrawPoint(sdl->renderer, x - 1, y - 1);
}

float	get_dist(int p_x, int p_y, float r_x, float r_y)
{
	float d_x;
	float d_y;

	d_x = p_x - r_x;
	d_y = p_y - r_y;
	return (sqrtf(d_x * d_x + d_y * d_y));
}

t_ray	*empty_ray(t_ray *ray)
{
	ray->dist = 99999;
	ray->offset = 0;
	ray->texture = NULL;
	return (ray);
}

int		is_wall_h(t_map *map, t_fpoint *pos, t_ray *ray, double angle)
{
	int		y;
	int		x;

	y = pos->y / 64;
	x = pos->x / 64;
	if (pos->x < 0 || pos->y < 0)
		return (1);
	if (pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	if (sin(angle) > 0)
		ray->texture = map->nodes[y][x].texture_w;
	else
		ray->texture = map->nodes[y][x].texture_e;
	return (map->nodes[y][x].collidable);
}

int		is_wall_v(t_map *map, t_fpoint *pos, t_ray *ray, double angle)
{
	int		y;
	int		x;

	y = pos->y / 64;
	x = pos->x / 64;
	if (pos->x < 0 || pos->y < 0)
		return (1);
	if (pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	if (cos(angle) < 0)
		ray->texture = map->nodes[y][x].texture_n;
	else
		ray->texture = map->nodes[y][x].texture_s;
	return (map->nodes[y][x].collidable);
}

t_ray	*cast_ray_horz(t_map *map, t_player *player, double angle)
{
	t_fpoint	end;
	t_fpoint	step;
	t_ray		*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	angle = angle * M_PI_180;
	if (sin(angle) < 0)
	{
		end.y = (player->y / 64) * 64 - 1;
		step.y = -64;
	}
	else if (sin(angle) > 0)
	{
		end.y = (player->y / 64) * 64 + 64;
		step.y = 64;
	}
	else
		return (empty_ray(ray));
	step.x = 64 / tan(angle);
	step.x = (cos(angle) <= 0) ? -fabsf(step.x) : fabsf(step.x);
	end.x = player->x + (end.y - player->y) / tan(angle);
	while (!is_wall_h(map, &end, ray, angle))
	{
		end.x += step.x;
		end.y += step.y;
	}
	//ray->dist = get_dist(player->x, player->y, start.x, start.y); // old method
	ray->dist = fabs((player->y - end.y) / sin(angle)); // new method
	ray->offset = (int)end.x % 64;
	return (ray);
}

t_ray	*cast_ray_vert(t_map *map, t_player *player, double angle)
{
	t_fpoint	end;
	t_fpoint	step;
	t_ray		*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	angle = angle * M_PI_180;
	if (cos(angle) > 0)
	{
		end.x = (player->x / 64) * 64 + 64;
		step.x = 64;
	}
	else if (cos(angle) < 0)
	{
		end.x = (player->x / 64) * 64 - 1;
		step.x = -64;
	}
	else
		return (empty_ray(ray));
	step.y = 64 * tan(angle);
	step.y = (sin(angle) >= 0) ? fabsf(step.y) : -fabsf(step.y);
	end.y = player->y + (end.x - player->x) * tan(angle);
	while (!is_wall_v(map, &end, ray, angle))
	{
		end.x += step.x;
		end.y += step.y;
	}
	//ray->dist = get_dist(player->x, player->y, start.x, start.y); // old method
	ray->dist = fabs((player->x - end.x) / cos(angle)); // new method
	ray->offset = (int)end.y % 64;
	return (ray);
}

t_ray	get_ray_length(t_map *map, t_player *player, double angle)
{
	t_ray		*ray_vert;
	t_ray		*ray_horz;
	t_ray		ray;

	ray_horz = cast_ray_horz(map, player, angle);
	ray_vert = cast_ray_vert(map, player, angle);
	ray = ray_horz->dist < ray_vert->dist ? *ray_horz : *ray_vert;
	ray.dist *= cos((angle - player->direction) * M_PI_180);
	free(ray_horz);
	free(ray_vert);
	return (ray);
}

t_ray	*get_ray_length2(t_map *map, t_player *player, double angle) // bugged. don't know why doesnt work
{
	t_ray		*ray_vert;
	t_ray		*ray_horz;
	t_ray		*ray;

	ray_horz = cast_ray_horz(map, player, angle);
	ray_vert = cast_ray_vert(map, player, angle);
	ray = ray_horz->dist < ray_vert->dist ? ray_horz : ray_vert;
	ray->dist *= cos((angle - player->direction) * M_PI_180);
	free(ray_horz);
	free(ray_vert);
	return (ray);
}

void		cast_single_ray(t_app *app, int x, float angle)
{
	int			slice_height;
	t_ray		ray;
	//t_ray		*ray2;

	ray = get_ray_length(app->map, app->player, angle);
	slice_height = 64 / ray.dist * app->sdl->dist_to_pp;
	draw_column(app->sdl, &ray, x, slice_height);
}

void 		create_hud(t_sdl *sdl)
{
	int 	x;
	int 	y;
	int		offset;


	y = sdl->height - sdl->height * 0.1;
	SDL_Color	color;

	color.r = 34;
	color.g = 46;
	color.b = 255;
	while (y < sdl->height)
	{
		x = 0;
		while (x < sdl->width)
		{
			offset = y * sdl->width + x;
			sdl->pixels[offset] = (Uint32)((color.r << 16) + (color.g << 8) + (color.b));
			x++;
		}
		y++;
	}
}

void		create_field_of_view(t_app *app)
{
	float	angle;
	float	next_angle;
	int		x;

	angle = app->player->direction - 30.0;
	next_angle = 60.0 / app->sdl->width;
	x = 0;
	while (x < app->sdl->width)
	{
		cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
	create_hud(app->sdl);
}
