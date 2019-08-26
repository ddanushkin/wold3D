/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:39 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:27:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_node	*get_node(t_node **nodes, float x, float y)
{
	int ix;
	int iy;

	iy = (int)y / TEXTURE_SIZE;
	ix = (int)x / TEXTURE_SIZE;
	return (&nodes[iy][ix]);
}

t_ray	*choose_ray(t_app *app, t_ray *horz, t_ray *vert, float angle)
{
	while (!is_wall(app->map, horz))
	{
		horz->start.x += horz->step.x;
		horz->start.y += horz->step.y;
	}
	while (!is_wall(app->map, vert))
	{
		vert->start.x += vert->step.x;
		vert->start.y += vert->step.y;
	}
	horz->dist = fabsf((app->player->y - horz->start.y) / sinf(angle));
	vert->dist = fabsf((app->player->x - vert->start.x) / cosf(angle));
	if (horz->dist <= vert->dist)
	{
		free(vert);
		return (horz);
	}
	else
	{
		free(horz);
		return (vert);
	}
}

t_ray	*get_ray(t_app *app, int x, float angle)
{
	t_ray	*vert;
	t_ray	*horz;
	t_ray	*result;

	angle = angle * M_PI_180;
	horz = init_horz(app->player->x, app->player->y, angle);
	vert = init_vert(app->player->x, app->player->y, angle);
	result = choose_ray(app, horz, vert, angle);
	if (result->node->type == MAP_TYPE_DOOR)
		calc_door_data(result);
	else
		calc_wall_data(result, angle);
	result->dist *= cos(angle - app->player->direction * M_PI_180);
	app->sdl->dist_per_x[x] = result->dist;
	return (result);
}

void	cast_single_ray(t_app *app, int x, float angle)
{
	t_ray	*ray;

	ray = get_ray(app, x, angle);
	ray->height = TEXTURE_SIZE / ray->dist * app->sdl->dist_to_pp;
	draw_column(app, ray, x, angle);
	free(ray);
}

void	create_field_of_view(t_app *app)
{
	float	angle;
	float	next_angle;
	int		x;

	angle = app->player->direction - (float)(60 * 0.5);
	next_angle = 60.0 / app->sdl->width;
	x = 0;
	while (x < app->sdl->width)
	{
		cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
}
