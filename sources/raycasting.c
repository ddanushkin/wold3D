/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:21:40 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/20 15:17:59 by lglover          ###   ########.fr       */
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

void	set_wall_texture(t_map *map, t_fpoint *pos, t_ray *ray, double angle)
{
	int		y;
	int		x;

	y = pos->y / 64;
	x = pos->x / 64;
	if ((ft_strcmp(ray->type, "horz")) == 0)
	{
		if (sin(angle) > 0)
			ray[ray[0].count].texture = map->nodes[y][x].texture[2];
		else
			ray[ray[0].count].texture = map->nodes[y][x].texture[1];
	}
	else
	{
		if (cos(angle) < 0)
			ray[ray[0].count].texture = map->nodes[y][x].texture[0];
		else
			ray[ray[0].count].texture = map->nodes[y][x].texture[3];
	}
}

int		is_wall(t_map *map, t_fpoint *pos, t_ray *ray, double angle, t_player *player)
{
	int		y;
	int		x;

	if (pos->x < 0 || pos->y < 0)
		return (1);
	if (pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	y = pos->y / 64;
	x = pos->x / 64;
	if (map->nodes[y][x].type == 2)
	{
		ray[ray[0].count].texture = map->nodes[y][x].texture[0];
		ray[ray[0].count].offset = (int)pos->x % 64;
		ray[ray[0].count].dist = sqrt(ft_powd(player->x - map->nodes[y][x].center.x, 2) + ft_powd(player->y - map->nodes[y][x].center.y, 2));
		ray[0].count++;
	}
	if (map->nodes[y][x].type == 1)
		set_wall_texture(map, pos, ray, angle);
	return (map->nodes[y][x].type == 1);
}

t_ray	*cast_ray_horz(t_map *map, t_player *player, double angle)
{
	t_fpoint	end;
	t_fpoint	step;
	t_ray		*ray;

	ray = (t_ray *)malloc(sizeof(t_ray) * 10);
	ray->type = "horz";
	ray->count = 0;
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
	while (!is_wall(map, &end, ray, angle, player))
	{
		end.x += step.x;
		end.y += step.y;
	}
	ray[ray[0].count].dist = fabs((player->y - end.y) / sin(angle));
	ray[ray[0].count].offset = (int)end.x % 64;
	ray[0].count++;
	return (ray);
}

t_ray	*cast_ray_vert(t_map *map, t_player *player, double angle)
{
	t_fpoint	end;
	t_fpoint	step;
	t_ray		*ray;

	ray = (t_ray *)malloc(sizeof(t_ray) * 10);
	ray->type = "vert";
	ray->count = 0;
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
	while (!is_wall(map, &end, ray, angle, player))
	{
		end.x += step.x;
		end.y += step.y;
	}
	ray[ray[0].count].dist = fabs((player->x - end.x) / cos(angle));
	ray[ray[0].count].offset = (int)end.y % 64;
	ray[0].count++;
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

float	cast_single_ray(t_app *app, int x, float angle)
{
	int		slice_height;
	t_ray	*rays;
	int 	i;

	rays = get_ray(app->map, app->player, angle);
	i = rays->count;
	while (i > 0)
	{
		slice_height = (int)(64 / rays[i - 1].dist * app->sdl->dist_to_pp);
		draw_column(app->sdl, &rays[i - 1], x, slice_height, i - 1);
		i--;
	}
	free(rays);
}

//void	draw_objects(t_app *app)
//{
//	int i;
//	float dist;
//	int slice_height;
//	t_ray ray;
//
//	i = 0;
//	while (i < app->map->obj_count)
//	{
//		dist = sqrt(ft_powd(app->player->x - app->map->objects[i].x, 2) + ft_powd(app->player->y - app->map->objects[i].y, 2));
//		slice_height = (int)(64 / dist * app->sdl->dist_to_pp);
//		ray.texture = app->map->objects[i].texture;
//		ray.dist = dist;
//		ray.offset = 0;
//		while (ray.offset < app->sdl->width)
//		{
//			draw_obj_column(app->sdl, &ray, app->map->objects[i].x + ray.offset, slice_height);
//			ray.offset++;
//		}
//		i++;
//	}
//}

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
		cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
	//draw_objects(app);
}
