/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:13 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:26:13 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		move_forward(t_node **nodes, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(player->speed * player->x_v)
			+ (player->x_v > 0 ? player->max_dist : -player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->x_v > 0 ? -player->max_dist : player->max_dist);
	old_y = player->y;
	player->y += (int)(player->speed * player->y_v)
			+ (player->y_v > 0 ? player->max_dist : -player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->y_v > 0 ? -player->max_dist : player->max_dist);
}

static void		move_backward(t_node **nodes, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(-player->speed * player->x_v)
			+ (player->x_v > 0 ? -player->max_dist : player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->x_v > 0 ? player->max_dist : -player->max_dist);
	old_y = player->y;
	player->y += (int)(-player->speed * player->y_v)
			+ (player->y_v > 0 ? -player->max_dist : player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->y_v > 0 ? player->max_dist : -player->max_dist);
}

static void		move_left(t_node **nodes, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(-player->speed * player->y_v)
			+ (player->y_v > 0 ? -player->max_dist : player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->y_v > 0 ? player->max_dist : -player->max_dist);
	old_y = player->y;
	player->y += (int)(player->speed * player->x_v)
			+ (player->x_v > 0 ? player->max_dist : -player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->x_v > 0 ? -player->max_dist : player->max_dist);
}

static void		move_right(t_node **nodes, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(player->speed * player->y_v)
			+ (player->y_v > 0 ? player->max_dist : -player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->y_v > 0 ? -player->max_dist : player->max_dist);
	old_y = player->y;
	player->y += (int)(-player->speed * player->x_v)
			+ (player->x_v > 0 ? -player->max_dist : player->max_dist);
	if (nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->x_v > 0 ? player->max_dist : -player->max_dist);
}

void			player_movement(t_node **nodes, const Uint8 *key,
								t_player *player)
{
	int old_x;
	int old_y;

	old_x = player->x;
	old_y = player->y;
	if (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_UP])
		move_forward(nodes, player);
	if (key[SDL_SCANCODE_S] || key[SDL_SCANCODE_DOWN])
		move_backward(nodes, player);
	if (key[SDL_SCANCODE_A] || key[SDL_SCANCODE_LEFT])
		move_right(nodes, player);
	if (key[SDL_SCANCODE_D] || key[SDL_SCANCODE_RIGHT])
		move_left(nodes, player);
	if (old_x != player->x || old_y != player->y)
	{
		if (player->head_acc < 2)
			player->head_acc += 0.35;
	}
	else
	{
		if (player->head_acc > 0)
			player->head_acc -= 0.15;
	}
}
