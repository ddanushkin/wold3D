/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 12:14:29 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/20 15:17:59 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static	void	player_shoot(t_player *player, float frame)
{
	if (player->weapon[player->cur_weapon].ammo_cur > 0)
	{
		printf("SHOOT: START\n");
		player->anim_frame = frame;
		player->anim_is_done = 0;
		player->shooting = 1;
		Mix_PlayChannel(-1, player->weapon[player->cur_weapon].gun_sound, 0);
	}
	/*else if (player->weapon[player->cur_weapon].ammo_cur == 0)
		Mix_PlayChannel(-1, player->fx_empty, 0);*/
}

static	void	player_change_weapon(t_player *player, float frame)
{
	printf("CHANGE: START\n");
	player->anim_frame = frame;
	player->anim_is_done = 0;
	player->changing = 1;
}

static	void	player_reloading(t_player *player, float frame)
{
	unsigned char	id;

	id = player->cur_weapon;
	if (player->weapon[id].ammo_cur < player->weapon[id].ammo_max && player->weapon[id].mag_cur > 0)
	{
		printf("RELOAD: START\n");
		player->anim_frame = frame;
		player->anim_is_done = 0;
		player->reloading = 1;
		Mix_PlayChannel(-1, player->fx_reload, 0);
	}
}

void			keyboard_input(t_app *wolf, const Uint8 *key, float frame)
{
	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(wolf->player, key);
	if (key[SDL_SCANCODE_SPACE] && wolf->player->anim_frame == 0)
		player_shoot(wolf->player, frame);
	if (key[SDL_SCANCODE_Q] && wolf->player->anim_frame == 0)
		player_change_weapon(wolf->player, frame);
	if (key[SDL_SCANCODE_R] && wolf->player->anim_frame == 0)
		player_reloading(wolf->player, frame);
	player_movement(wolf->map, key, wolf->player);
	if (key[SDL_SCANCODE_M] || key[SDL_SCANCODE_P])
		update_sound(key, wolf->player);
}
