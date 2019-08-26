/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:22:44 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:22:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		auto_close(t_app *app, t_node *door_node, float frame)
{
	int dist_to_player;

	dist_to_player = 0;
	if (door_node->last_open != 0)
		dist_to_player = abs(door_node->center.x - app->player->x) +
				abs(door_node->center.y - app->player->y);
	if (!door_node->door_opening && door_node->door_frame == 64 &&
		frame - door_node->last_open >= 5 && dist_to_player >= 60)
	{
		Mix_PlayChannel(2, app->sfx->door_move, 0);
		door_node->last_open = 0;
		door_node->door_closing = true;
	}
}

void			update_doors(t_app *app, float frame)
{
	t_node		*door_node;
	int			i;

	i = 0;
	while (i < app->map->doors_count)
	{
		door_node = app->map->doors[i];
		auto_close(app, door_node, frame);
		door_node->door_frame -= door_node->door_closing ? 1 : 0;
		door_node->door_frame += door_node->door_opening ? 1 : 0;
		if (door_node->door_frame == 59 && door_node->door_opening)
			Mix_PlayChannel(2, app->sfx->door_open, 0);
		if (door_node->door_frame == 5 && door_node->door_closing)
			Mix_PlayChannel(2, app->sfx->door_open, 0);
		if (door_node->door_frame > 64)
		{
			door_node->door_opening = false;
			door_node->last_open = frame;
			door_node->door_frame = 64;
		}
		if (door_node->door_frame == 0)
			door_node->door_closing = false;
		door_node->collidable = door_node->door_frame > 32 ? false : true;
		i++;
	}
}

void			door_interaction(t_app *app, float frame)
{
	t_ipoint	near_coord;
	t_node		*near_node;

	app->player->last_space = frame;
	near_coord.x = app->player->x + cos(app->player->direction * M_PI_180) * 64;
	near_coord.y = app->player->y + sin(app->player->direction * M_PI_180) * 64;
	near_node = &app->map->nodes[near_coord.y / 64][near_coord.x / 64];
	if (near_node->type == MAP_TYPE_DOOR)
	{
		Mix_PlayChannel(2, app->sfx->door_move, 0);
		if (!near_node->door_closing && !near_node->door_opening)
		{
			if (near_node->door_frame == 0)
				near_node->door_opening = true;
			if (near_node->door_frame == 64)
				near_node->door_closing = true;
		}
		else
		{
			near_node->door_opening = !near_node->door_opening;
			near_node->door_closing = !near_node->door_closing;
		}
	}
}
