/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:25:24 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:25:24 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_ray	*init_horz(int x, int y, float angle)
{
	t_ray	*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	ray->dist = 999999;
	ray->type = RAY_TYPE_HORZ;
	if (sinf(angle) < 0)
	{
		ray->start.y = (y & 0xffc0) - 0.001;
		ray->step.y = -64;
	}
	else if (sinf(angle) > 0)
	{
		ray->start.y = (y & 0xffc0) + 64.001;
		ray->step.y = 64;
	}
	else
		return (ray);
	ray->step.x = -ray->step.y / tanf(-angle);
	ray->start.x = x + (ray->start.y - y) / tanf(angle);
	return (ray);
}

t_ray	*init_vert(int x, int y, float angle)
{
	t_ray	*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	ray->dist = 999999;
	ray->type = RAY_TYPE_VERT;
	if (cosf(angle) > 0)
	{
		ray->start.x = (x & 0xffc0) + 64.001;
		ray->step.x = 64;
	}
	else if (cosf(angle) < 0)
	{
		ray->start.x = (x & 0xffc0) - 0.001;
		ray->step.x = -64;
	}
	else
		return (ray);
	ray->step.y = -ray->step.x * tanf(-angle);
	ray->start.y = y + (ray->start.x - x) * tanf(angle);
	return (ray);
}
