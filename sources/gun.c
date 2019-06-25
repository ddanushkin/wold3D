/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 16:40:34 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/25 14:56:55 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	gun_shoot(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;

	cur_frame = (int)(delta * 14.0) % 11 + 1;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 550;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("SHOOT: FRAME %d\n", cur_frame);
	if (cur_frame == 3 && !player->weapon[id].fired)
	{
		player->weapon[id].fired++;
		player->weapon[id].ammo_cur--;
	}
	else if (cur_frame > 10)
	{
		player->anim_is_done = 1;
		SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1], NULL, &area);
		return ;
	}
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[cur_frame], NULL, &area);
}

void	gun_change_down(t_sdl *sdl, t_player *player, unsigned char next_id, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;

	cur_frame = (long)(delta * 220.0) % 440;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 550 + cur_frame;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("CHANGE: FRAME %d\n", cur_frame);
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1], NULL, &area);
	if (area.y > 250)
	{
		player->change_down = 1;
		player->cur_weapon = next_id;
	}
}

void	gun_change_up(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;

	cur_frame = (long)(delta * 220.0) % 440;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 180 - cur_frame;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("CHANGE: FRAME %d\n", cur_frame);
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1], NULL, &area);
	if (area.y < 40)
	{
		player->anim_is_done = 1;
		player->change_down = 0;
		return ;
	}
}

void	gun_change(t_sdl *sdl, t_player *player, float frame)
{
	unsigned char	next_id;

	next_id = player->cur_weapon + 1;
	if (next_id == 3)
		next_id = 0;
	if (player->change_down == 0)
		gun_change_down(sdl, player, next_id, frame);
	if (player->change_down == 1)
		gun_change_up(sdl, player, frame);
}

void	gun_reload_down(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;
	unsigned char	old_ammo;

	cur_frame = (long)(delta * 220.0) % 440;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 550 + cur_frame;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("RELOAD: FRAME %d\n", cur_frame);
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1], NULL, &area);
	if (area.y > 250)
	{
		player->change_down = 1;
		old_ammo = player->weapon[id].ammo_cur;
		if (player->weapon[id].mag_cur >= player->weapon[id].ammo_max)
		{
			player->weapon[id].ammo_cur = player->weapon[id].ammo_max;
			player->weapon[id].mag_cur -= player->weapon[id].ammo_max - old_ammo;
		}
		else
		{
			player->weapon[id].ammo_cur = player->weapon[id].mag_cur;
			player->weapon[id].mag_cur = 0;
		}
	}
}

void	gun_reload(t_sdl *sdl, t_player *player, float frame)
{
	if (player->change_down == 0)
		gun_reload_down(sdl, player, frame);
	if (player->change_down == 1)
		gun_change_up(sdl, player, frame);
}
