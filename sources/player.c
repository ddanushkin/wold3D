/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 17:57:04 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/22 18:01:10 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	player_init(t_player *player)
{
	player->direction = 270;
	player->x_v = 0;
	player->y_v = 0;
}

void	player_rotate(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		(player->direction -= player->speed / 2) < 0 ? player->direction = 357 : 0;
	if (state[SDL_SCANCODE_RIGHT])
		(player->direction += player->speed / 2) > 359 ? player->direction = 4 : 0;
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT])
		printf("player_dir - %d\n", player->direction);
}

void	try_move(t_map *map, t_player *player, int new_x, int new_y)
{
	if (!map->nodes[new_y / 64][new_x / 64].collidable)
	{
		player->x = new_x;
		player->y = new_y;
	}
}

void	move_player(t_map *map, const Uint8 *key, t_player *player)
{
	int new_x;
	int new_y;

	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(player, key);
	player->x_v = cos(player->direction * M_PI_180);
	player->y_v = sin(player->direction * M_PI_180);
	if (key[SDL_SCANCODE_W])
	{
		new_x = player->x + (int)(player->speed * player->x_v);
		new_y = player->y + (int)(player->speed * player->y_v);
		try_move(map, player, new_x, new_y);
	}
	if (key[SDL_SCANCODE_S])
	{
		new_x = player->x - (int)(player->speed * player->x_v);
		new_y = player->y - (int)(player->speed * player->y_v);
		try_move(map, player, new_x, new_y);
	}
	if (key[SDL_SCANCODE_D])
	{
		new_x = player->x - (int)(player->speed * player->y_v);
		new_y = player->y + (int)(player->speed * player->x_v);
		try_move(map, player, new_x, new_y);
	}
	if (key[SDL_SCANCODE_A])
	{
		new_x = player->x + (int)(player->speed * player->y_v);
		new_y = player->y - (int)(player->speed * player->x_v);
		try_move(map, player, new_x, new_y);
	}
}
