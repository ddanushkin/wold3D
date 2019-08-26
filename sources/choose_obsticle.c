/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_obsticle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:21:53 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 15:11:47 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		is_door(t_ray *ray, t_node *node)
{
	ray->start.x += ray->step.x * 0.5;
	ray->start.y += ray->step.y * 0.5;
	if (ray->type == RAY_TYPE_HORZ &&
		(int)(ray->start.x) % TEXTURE_SIZE < node->door_frame)
	{
		ray->start.x -= ray->step.x * 0.5;
		ray->start.y -= ray->step.y * 0.5;
		return (0);
	}
	if (ray->type == RAY_TYPE_VERT &&
		(int)(ray->start.y) % TEXTURE_SIZE < node->door_frame)
	{
		ray->start.x -= ray->step.x * 0.5;
		ray->start.y -= ray->step.y * 0.5;
		return (0);
	}
	ray->node = node;
	return (1);
}

int		is_wall(t_map *map, t_ray *ray)
{
	t_node *node;

	if (ray->start.x < 0 || ray->start.y < 0)
		return (1);
	if (ray->start.x >= map->cols * 64 || ray->start.y >= map->rows * 64)
		return (1);
	node = get_node(map->nodes, ray->start.x, ray->start.y);
	if (node->type == MAP_TYPE_EMPTY)
		return (0);
	if (node->type == MAP_TYPE_DOOR)
		return (is_door(ray, node));
	if (node->type == MAP_TYPE_WALL)
	{
		ray->node = node;
		return (1);
	}
	return (0);
}

void	calc_wall_data(t_ray *ray, float angle)
{
	if (ray->type == RAY_TYPE_HORZ)
	{
		ray->offset = (int)ray->start.x % 64;
		ray->texture = ray->node->texture[(sinf(angle) > 0 ? 0 : 1)];
	}
	else
	{
		ray->offset = (int)ray->start.y % 64;
		ray->texture = ray->node->texture[(cosf(angle) < 0 ? 3 : 2)];
	}
}

void	calc_door_data(t_ray *ray)
{
	ray->texture = ray->node->texture[0];
	if (ray->type == RAY_TYPE_HORZ)
		ray->offset = (int)(ray->start.x) % TEXTURE_SIZE;
	else
		ray->offset = (int)(ray->start.y) % TEXTURE_SIZE;
	ray->offset -= ray->node->door_frame;
}
