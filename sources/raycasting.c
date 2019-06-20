/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:21:40 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/20 18:55:46 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_ray_data	*empty_ray(t_ray_data *ray)
{
	ray->dist = 999999;
	return (ray);
}

t_node		*get_node(t_map *map, float x, float y)
{
	int ix;
	int iy;

	iy = (int)y / 64;
	ix = (int)x / 64;
	return (&map->nodes[iy][ix]);
}

t_ray_data	*get_horz_dist(t_player *player, double angle)
{
	t_ray_data	*ray;

	ray = (t_ray_data *)malloc(sizeof(t_ray_data));
	angle = angle * M_PI_180;
	if (sin(angle) < 0)
	{
		ray->start.y = (int)(player->y / 64) * 64.0 - 0.001;
		ray->step.y = -64;
	}
	else if (sin(angle) > 0)
	{
		ray->start.y = (int)(player->y / 64) * 64.0 + 64.001;
		ray->step.y = 64;
	}
	else
	{
		ray->dist = 999999;
		return (ray);
	}
	ray->step.x = 64 / tan(angle);
	ray->step.x = (cos(angle) <= 0) ? -fabsf(ray->step.x) : fabsf(ray->step.x);
	ray->start.x = player->x + (ray->start.y - player->y) / tan(angle);
	ray->dist = fabs((player->y - ray->start.y) / sin(angle));
	return (ray);
}

t_ray_data	*get_vert_dist(t_player *player, double angle)
{
	t_ray_data	*ray;

	ray = (t_ray_data *)malloc(sizeof(t_ray_data));
	angle = angle * M_PI_180;
	if (cos(angle) > 0)
	{
		ray->start.x = player->x / 64 * 64 + 64.001;
		ray->step.x = 64;
	}
	else if (cos(angle) < 0)
	{
		ray->start.x = player->x / 64 * 64 - 0.001;
		ray->step.x = -64;
	}
	else
		return (empty_ray(ray));
	ray->step.y = 64 * tan(angle);
	ray->step.y = (sin(angle) >= 0) ? fabsf(ray->step.y) : -fabsf(ray->step.y);
	ray->start.y = player->y + (ray->start.x - player->x) * tan(angle);
	ray->dist = fabs((player->x - ray->start.x) / cos(angle));
	return (ray);
}

int		check_horz(t_map *map, t_player *player, t_ray_data *ray, double angle)
{
	t_node *node;

	if (ray->start.x < 0 || ray->start.y < 0)
		return (1);
	if (ray->start.x >= map->cols * 64 || ray->start.y >= map->rows * 64)
		return (1);
	node = get_node(map, ray->start.x, ray->start.y);
	if (node->type == MAP_TYPE_EMPTY)
		return (0);
	if (node->type == MAP_TYPE_INTERIOR)
		ray->objects[ray->count].texture = node->texture[0];
	else if (node->type == MAP_TYPE_WALL)
	{
		if (sin(angle) > 0)
			ray->objects[ray->count].texture = node->texture[2];
		else
			ray->objects[ray->count].texture = node->texture[1];
	}
	ray->objects[ray->count].offset = (int)ray->start.x % 64;
	ray->objects[ray->count].dist = fabs((player->y - ray->start.y) / sin(angle));
	ray->objects[ray->count].dist *= cos(angle - player->direction * M_PI_180);
	ray->count++;
	if (node->type == MAP_TYPE_WALL)
		return (1);
	return (0);
}

int		check_vert(t_map *map, t_player *player,  t_ray_data *ray, double angle)
{
	t_node *node;

	node = get_node(map, ray->start.x, ray->start.y);
	if (node->type == MAP_TYPE_EMPTY)
		return (0);
	if (node->type == MAP_TYPE_INTERIOR)
		ray->objects[ray->count].texture = node->texture[0];
	else if (node->type == MAP_TYPE_WALL)
	{
		if (cos(angle) < 0)
			ray->objects[ray->count].texture = node->texture[0];
		else
			ray->objects[ray->count].texture = node->texture[3];
	}
	ray->objects[ray->count].offset = (int)ray->start.y % 64;
	ray->objects[ray->count].dist = fabs((player->x - ray->start.x) / cos(angle));
	ray->objects[ray->count].dist *= cos(angle - player->direction * M_PI_180);
	ray->count++;
	if (node->type == MAP_TYPE_WALL)
		return (1);
	return (0);
}

t_ray_data	*cast_ray_horz(t_map *map, t_player *player, double angle, t_ray_data *ray)
{
	ray->objects = (t_cross *)malloc(sizeof(t_cross) * 10);
	ray->count = 0;
	angle = angle * M_PI_180;
	while (!check_horz(map, player, ray, angle))
	{
		ray->start.x += ray->step.x;
		ray->start.y += ray->step.y;
	}
	return (ray);
}

t_ray_data	*cast_ray_vert(t_map *map, t_player *player, double angle, t_ray_data *ray)
{
	ray->objects = (t_cross *)malloc(sizeof(t_cross) * 10);
	ray->count = 0;
	angle = angle * M_PI_180;
	while (!check_vert(map, player, ray, angle))
	{
		ray->start.x += ray->step.x;
		ray->start.y += ray->step.y;
	}
	return (ray);
}

t_ray_data	*get_ray(t_app *app, double angle)
{
	t_ray_data	*vert;
	t_ray_data	*horz;
	
	horz = get_horz_dist(app->player, angle);
	vert = get_vert_dist(app->player, angle);
	if (horz->dist <= vert->dist)
	{
		free(vert);
		return (cast_ray_horz(app->map, app->player, angle, horz));
	}
	else
	{
		free(horz);
		return (cast_ray_vert(app->map, app->player, angle, vert));
	}
}

void	cast_single_ray(t_app *app, int x, float angle)
{
	int			slice_height;
	t_ray_data	*ray;
	int 		i;

	ray = get_ray(app, angle);
	i = ray->count;
	while (i > 0)
	{
		slice_height = (int)(64 / ray->objects[i - 1].dist * app->sdl->dist_to_pp);
		draw_column(app->sdl, &ray->objects[i - 1], x, slice_height);
		i--;
	}
	free(ray->objects);
	free(ray);
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
		cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
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

//t_ray	*cast_ray_horz(t_map *map, t_player *player, double angle)
//{
//	t_fpoint	end;
//	t_fpoint	step;
//	t_ray		*ray;
//
//	ray = (t_ray *)malloc(sizeof(t_ray) * 10);
//	ray->type = "horz";
//	ray->count = 0;
//	angle = angle * M_PI_180;
//	if (sin(angle) < 0)
//	{
//		end.y = (int)(player->y / 64) * 64.0 - 0.001;
//		step.y = -64;
//	}
//	else if (sin(angle) > 0)
//	{
//		end.y = (int)(player->y / 64) * 64.0 + 64.001;
//		step.y = 64;
//	}
//	else
//		return (empty_ray(ray));
//	step.x = 64 / tan(angle);
//	step.x = (cos(angle) <= 0) ? -fabsf(step.x) : fabsf(step.x);
//	end.x = player->x + (end.y - player->y) / tan(angle);
//	while (!is_wall(map, &end, ray, angle, player))
//	{
//		end.x += step.x;
//		end.y += step.y;
//	}
//	ray[ray[0].count].dist = fabs((player->y - end.y) / sin(angle));
//	ray[ray[0].count].dist *= cos(angle - player->direction * M_PI_180);
//	ray[ray[0].count].offset = (int)end.x % 64;
//	ray[0].count++;
//	return (ray);
//}
//
//t_ray	*cast_ray_vert(t_map *map, t_player *player, double angle)
//{
//	t_fpoint	end;
//	t_fpoint	step;
//	t_ray		*ray;
//
//	ray = (t_ray *)malloc(sizeof(t_ray) * 10);
//	ray->type = "vert";
//	ray->count = 0;
//	angle = angle * M_PI_180;
//	if (cos(angle) > 0)
//	{
//		end.x = player->x / 64 * 64 + 64.001;
//		step.x = 64;
//	}
//	else if (cos(angle) < 0)
//	{
//		end.x = player->x / 64 * 64 - 0.001;
//		step.x = -64;
//	}
//	else
//		return (empty_ray(ray));
//	step.y = 64 * tan(angle);
//	step.y = (sin(angle) >= 0) ? fabsf(step.y) : -fabsf(step.y);
//	end.y = player->y + (end.x - player->x) * tan(angle);
//	while (!is_wall(map, &end, ray, angle, player))
//	{
//		end.x += step.x;
//		end.y += step.y;
//	}
//	ray[ray[0].count].dist = fabs((player->x - end.x) / cos(angle));
//	ray[ray[0].count].dist *= cos(angle - player->direction * M_PI_180);
//	ray[ray[0].count].offset = (int)end.y % 64;
//	ray[0].count++;
//	return (ray);
//}

//int		is_wall(t_map *map, t_player *player, t_fpoint pos, t_ray_data *ray, double angle)
//{
//	int		y;
//	int		x;
//
//	if (pos.x < 0 || pos.y < 0)
//		return (1);
//	if (pos.x >= map->cols * 64 || pos.y >= map->rows * 64)
//		return (1);
//	y = pos.y / 64;
//	x = pos.x / 64;
//
//	if (map->nodes[y][x].type == 2)
//	{
//		ray->objects[ray->count].texture = map->nodes[y][x].texture[0];
//		ray->objects[ray->count].offset = (ft_strcmp(ray.type, "horz")) == 0 ?
//										  (int)pos.x % 64 :
//										  (int)pos.y % 64;
//		ray[ray[0].count].dist = (ft_strcmp(ray.type, "horz")) == 0 ?
//								 fabs((player.y - pos.y) / sin(angle)) :
//								 fabs((player.x - pos.x) / cos(angle));
//		ray[ray[0].count].dist += (ray[ray[0].count].offset >= 32 ?
//								   -(player.direction % 180) * ray[ray[0].count].offset % 64:
//								   (player.direction % 180)) * ray[ray[0].count].offset % 64;
//		ray[ray[0].count].dist *= cos(angle - player.direction * M_PI_180);
//		ray[0].count++;
//	}
//	if (map.nodes[y][x].type == 1)
//		set_wall_texture(map, pos, ray, angle);
//	return (map.nodes[y][x].type == 1);
//}

//void	set_wall_texture(t_map *map, t_player *player, t_ray_data *ray, double angle)
//{
//	int		y;
//	int		x;
//
//	y = ray->start.y / 64;
//	x = ray->start.x / 64;
//	ray->objects[ray->count].dist = fabs((player->y - ray->start.y) / sin(angle));
//	ray->objects[ray->count].dist *= cos(angle - player->direction * M_PI_180);
//	ray->objects[ray->count].offset = (int)end.x % 64;
//
//	if ((ft_strcmp(ray->type, "horz")) == 0)
//	{
//
//	}
//	else
//	{
//		if (cos(angle) < 0)
//			ray[ray[0].count].texture = map->nodes[y][x].texture[0];
//		else
//			ray[ray[0].count].texture = map->nodes[y][x].texture[3];
//	}
//}
