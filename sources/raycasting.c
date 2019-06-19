/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:21:40 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/19 15:22:51 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

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
		ray->texture = map->nodes[y][x].texture[2];
	else
		ray->texture = map->nodes[y][x].texture[1];
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
		ray->texture = map->nodes[y][x].texture[0];
	else
		ray->texture = map->nodes[y][x].texture[3];
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
		end.y = (int)(player->y / 64) * 64.0 - 0.001;
		step.y = -64;
	}
	else if (sin(angle) > 0)
	{
		end.y = (int)(player->y / 64) * 64.0 + 64.001;
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
	ray->dist = fabs((player->y - end.y) / sin(angle));
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
		end.x = player->x / 64 * 64 + 64.001;
		step.x = 64;
	}
	else if (cos(angle) < 0)
	{
		end.x = player->x / 64 * 64 - 0.001;
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
	ray->dist = fabs((player->x - end.x) / cos(angle));
	ray->offset = (int)end.y % 64;
	return (ray);
}

t_ray	*get_ray(t_map *map, t_player *player, double angle)
{
	t_ray	*ray_vert;
	t_ray	*ray_horz;

	ray_horz = cast_ray_horz(map, player, angle);
	ray_vert = cast_ray_vert(map, player, angle);
	if (ray_horz->dist <= ray_vert->dist)
	{
		ray_horz->dist *= cos(fabs(angle - player->direction) * M_PI_180);
		free(ray_vert);
		return (ray_horz);
	}
	else
	{
		ray_vert->dist *= cos(fabs(angle - player->direction) * M_PI_180);
		free(ray_horz);
		return (ray_vert);
	}
}

float	 cast_single_ray(t_app *app, int x, float angle)
{
	int			slice_height;
	t_ray		*ray;
	float 		dist;

	ray = get_ray(app->map, app->player, angle);
	slice_height = (int)(64 / ray->dist * app->sdl->dist_to_pp);
	draw_column(app->sdl, ray, x, slice_height);
	dist = ray->dist;
	free(ray);
	return (dist);
}

void draw_objects(t_app *app)
{
	int i;
	float dist;
	int slice_height;
	t_ray ray;

	i = 0;
	while (i < app->map->obj_count)
	{
		dist = sqrt((app->player->x - app->map->objects[i].x) * (app->player->x - app->map->objects[i].x)
				+ (app->player->y - app->map->objects[i].y) * (app->player->y - app->map->objects[i].y));
		slice_height = (int)(64 / dist * app->sdl->dist_to_pp);
		ray.texture = app->map->objects[i].texture;
		ray.dist = dist;
		ray.offset = 0;
		while (ray.offset < 64)
		{
			draw_column(app->sdl, &ray, app->map->objects[i].x + ray.offset, slice_height);
			ray.offset++;
		}
		i++;
	}
}

void	create_field_of_view(t_app *app)
{
	float	angle;
	float	next_angle;
	int		x;
	double	fov;

	fov = 60;
	angle = app->player->direction - (fov / 2.0);
	next_angle = fov / app->sdl->width;
	x = 0;
	while (x < app->sdl->width)
	{
		app->sdl->dist_per_x[x] = cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
	draw_objects(app);
}
