/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:21 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:26:21 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	fill_object(t_sdl *sdl, t_player *player, t_node *object)
{
	int		dx;
	int		dy;
	float	angle;

	dx = player->x - object->center.x;
	dy = player->y - object->center.y;
	object->visible = true;
	if (dy >= 0 && player->direction >= 35 && player->direction <= 145)
		object->visible = false;
	else if (dy < 0 && player->direction >= 215 && player->direction <= 325)
		object->visible = false;
	else if (dx >= 0 && ((player->direction >= 305 && player->direction <= 359)
	|| (player->direction >= 0 && player->direction <= 55)))
		object->visible = false;
	else if (dx < 0 && player->direction >= 125 && player->direction <= 235)
		object->visible = false;
	if (!object->visible)
		return ;
	angle = atan2f(dy, dx) - (player->direction * M_PI_180);
	object->dist = sqrtf(dx * dx + dy * dy);
	object->screen_x = tanf(angle) * sdl->dist_to_pp + sdl->half_width;
	object->height = (int)(TEXTURE_SIZE / object->dist * sdl->dist_to_pp);
}

static void	reset_objects(t_app *app)
{
	int		i;
	int		objects_count;
	t_node	*object;

	i = 0;
	objects_count = app->map->objects_count;
	while (i < objects_count)
	{
		object = app->map->objects[i];
		object->screen_x = -1;
		object->height = -1;
		object->dist = -1;
		i++;
	}
}

static void	sort_objects(t_app *app)
{
	int		i;
	int		objects_count;
	t_node	**objects;
	t_node	*tmp_object;

	objects_count = app->map->objects_count;
	objects = app->map->objects;
	i = 0;
	while (i < objects_count)
	{
		if (i + 1 < objects_count && objects[i]->dist < objects[i + 1]->dist)
		{
			tmp_object = objects[i + 1];
			objects[i + 1] = objects[i];
			objects[i] = tmp_object;
			i = -1;
		}
		i++;
	}
}

static void	draw_objects(t_app *app)
{
	int		i;
	int		objects_count;
	t_node	*object;

	objects_count = app->map->objects_count;
	i = 0;
	while (i < objects_count)
	{
		object = app->map->objects[i];
		if (object->visible)
			draw_object(app, object);
		i++;
	}
}

void		update_objects(t_app *app)
{
	int		i;
	int		objects_count;
	t_node	*object;

	reset_objects(app);
	objects_count = app->map->objects_count;
	i = 0;
	while (i < objects_count)
	{
		object = app->map->objects[i];
		fill_object(app->sdl, app->player, object);
		i++;
	}
	sort_objects(app);
	draw_objects(app);
}
