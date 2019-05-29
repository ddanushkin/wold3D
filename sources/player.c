/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 17:57:04 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/29 17:07:45 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	player_init(t_sdl *sdl, t_player *player)
{
	t_weapon *weapon;

	player->weapon = (t_weapon *)malloc(sizeof(t_weapon) * 2);
	player->direction = 90;
	player->lives = 99;
	player->health = 100;
	player->score = 0;
	player->cur_level = 0;
	player->bgm = Mix_LoadMUS("../resources/bgm.mp3");
	player->sound_effect = Mix_LoadWAV("../resources/speak.wav");
	player->state[0] = load_texture(sdl, "fpain L1");
	player->state[1] = load_texture(sdl, "fpain R1");

	player->weapon[0].ammo_cur = 10;
	player->weapon[0].ammo_max = 10;
	player->weapon[0].mag_cur = 10 * 4;
	player->weapon[0].mag_max = 10 * 4;
	player->weapon[0].firerate = 0.8;
	player->weapon[0].fired = 0;
	player->weapon[0].sprites[0] = load_texture(sdl, "Hunterfloor");
	player->weapon[0].sprites[1] = load_texture(sdl, "Hunter1");
	player->weapon[0].sprites[2] = load_texture(sdl, "Hunter2");
	player->weapon[0].sprites[3] = load_texture(sdl, "Hunter3");
	player->weapon[0].sprites[4] = load_texture(sdl, "Hunter4");
	player->weapon[0].sprites[5] = load_texture(sdl, "Hunter5");
	player->weapon[0].sprites[6] = load_texture(sdl, "Hunter6");
	player->weapon[0].sprites[7] = load_texture(sdl, "Hunter7");
	player->weapon[0].sprites[8] = load_texture(sdl, "Hunter8");
	player->weapon[0].sprites[9] = load_texture(sdl, "Hunter9");
	player->weapon[0].sprites[10] = load_texture(sdl, "Hunter10");

	player->weapon[1].ammo_cur = 6;
	player->weapon[1].ammo_max = 6;
	player->weapon[1].mag_cur = 6 * 4;
	player->weapon[1].mag_max = 6 * 4;
	player->weapon[1].firerate = 1.7;
	player->weapon[1].fired = 0;
	player->weapon[1].sprites[0] = load_texture(sdl, "Rustmagfloor");
	player->weapon[1].sprites[1] = load_texture(sdl, "Rustmag1");
	player->weapon[1].sprites[2] = load_texture(sdl, "Rustmag2");
	player->weapon[1].sprites[3] = load_texture(sdl, "Rustmag3");
	player->weapon[1].sprites[4] = load_texture(sdl, "Rustmag4");
	player->weapon[1].sprites[5] = load_texture(sdl, "Rustmag5");
	player->weapon[1].sprites[6] = load_texture(sdl, "Rustmag6");
	player->weapon[1].sprites[7] = load_texture(sdl, "Rustmag7");
	player->weapon[1].sprites[8] = load_texture(sdl, "Rustmag8");
	player->weapon[1].sprites[9] = load_texture(sdl, "Rustmag9");
	player->weapon[1].sprites[10] = load_texture(sdl, "Rustmag10");

	player->cur_weapon = 0;

	player->shooting = 0;
	player->changing = 0;
	player->reloading = 0;
	player->change_down = 0;
	player->anim_is_done = 1;
	player->anim_frame = 0;
}

void	draw_face(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;

	area.y = sdl->height - 130;
	area.w = 96;
	area.x = sdl->width / 2 - area.w + 5;
	area.h = 116;

	cur_frame = (long)delta % 2;
	SDL_RenderCopy(sdl->renderer, player->state[cur_frame], NULL, &area);
}

void	player_rotate(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		(player->direction -= player->speed * 0.5) < 0 ? player->direction = 359 : 0;
	if (state[SDL_SCANCODE_RIGHT])
		(player->direction += player->speed * 0.5) > 359 ? player->direction = 1 : 0;
}

float 	get_ray_length(t_map *map, t_player *player, double angle)
{
	float dist;
	t_ray *ray;

	angle = (long)(angle) % 360;
	ray = get_ray(map, player, angle);
	dist = ray->dist;
	free(ray);
	return (dist / cos(fabs(angle - player->direction) * M_PI_180));
}

void	player_update_dist(t_map *map, t_player *player)
{
	player->dist_n = get_ray_length(map, player, player->direction);
	player->dist_s = get_ray_length(map, player, player->direction - 180);
	player->dist_e = get_ray_length(map, player, player->direction - 90);
	player->dist_w = get_ray_length(map, player, player->direction + 90);
}

void	player_movement(t_map *map, const Uint8 *key, t_player *player)
{
	player_update_dist(map, player);
	player->x_v = cos(player->direction * M_PI_180);
	player->y_v = sin(player->direction * M_PI_180);
	if (key[SDL_SCANCODE_W] && player->dist_n > 30)
	{
		player->x += (int)(player->speed * player->x_v);
		player->y += (int)(player->speed * player->y_v);
	}
	if (key[SDL_SCANCODE_S] && player->dist_s > 30)
	{
		player->x -= (int)(player->speed * player->x_v);
		player->y -= (int)(player->speed * player->y_v);
	}
	if (key[SDL_SCANCODE_D] && player->dist_w > 30)
	{
		player->x -= (int)(player->speed * player->y_v);
		player->y += (int)(player->speed * player->x_v);
	}
	if (key[SDL_SCANCODE_A] && player->dist_e > 30)
	{
		player->x += (int)(player->speed * player->y_v);
		player->y -= (int)(player->speed * player->x_v);
	}
}

void	update_sound(const Uint8 *key, t_player *player)
{
	if (key[SDL_SCANCODE_M])
	{
		if (!Mix_PlayingMusic())
			Mix_PlayMusic(player->bgm, -1);
		else if (Mix_PausedMusic())
			Mix_ResumeMusic();
	}
	if (key[SDL_SCANCODE_P])
		Mix_PauseMusic();
	//if (key[SDL_SCANCODE_SPACE])
		//Mix_PlayChannel(-1, player->sound_effect, 0);
}

void	gun_idle(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;

	id = player->cur_weapon;
	cur_frame = (long)(delta * 1.9);
	area.y = sdl->height - 130 - 550 + (cur_frame % 2 * 5);
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;

	cur_frame %= 2;
	if (cur_frame == 0)
		cur_frame++;
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[cur_frame], NULL, &area);
}

void	gun_shoot(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	float			cur_frame;
	unsigned char	id;

	cur_frame = (long)(delta * 14.0) % 11 + 1;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 550;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("SHOOT: FRAME %d\n", (int)cur_frame);
	if (cur_frame == 3 && !player->weapon[id].fired)
	{
		player->weapon[id].fired++;
		player->weapon[id].ammo_cur--;
	}
	if (cur_frame > 10)
	{
		player->anim_is_done = 1;
		SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1], NULL, &area);
		return ;
	}
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[(int)cur_frame], NULL, &area);
}

void 	gun_change_down(t_sdl *sdl, t_player *player, unsigned char next_id, float delta)
{
	SDL_Rect		area;
	float			cur_frame;
	unsigned char	id;

	cur_frame = (long)(delta * 220.0) % 440;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 550 + (int)cur_frame;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("CNANGE: FRAME %d\n", (int)cur_frame);
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1], NULL, &area);
	if (area.y > 250)
	{
		player->change_down = 1;
		player->cur_weapon = next_id;
	}
}

void 	gun_change_up(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	float			cur_frame;
	unsigned char	id;

	cur_frame = (long)(delta * 220.0) % 440;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 180 - (int)cur_frame;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("CNANGE: FRAME %d\n", (int)cur_frame);
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1], NULL, &area);
	if (area.y < 40)
	{
		player->anim_is_done = 1;
		player->change_down = 0;
		return ;
	}
}

void 	gun_change(t_sdl *sdl, t_player *player, unsigned int next_weapon, float frame)
{
	unsigned char next_id;

	next_id = player->cur_weapon ? 0 : 1;
	if (player->change_down == 0)
		gun_change_down(sdl, player, next_id, frame);
	if (player->change_down == 1)
		gun_change_up(sdl, player,  frame);
}

void 	gun_reload_down(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	float			cur_frame;
	unsigned char	id;
	unsigned char	old_ammo;

	cur_frame = (long)(delta * 220.0) % 440;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 550 + (int)cur_frame;
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	printf("RELOAD: FRAME %d\n", (int)cur_frame);
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

void 	gun_reload(t_sdl *sdl, t_player *player, float frame)
{
	if (player->change_down == 0)
		gun_reload_down(sdl, player, frame);
	if (player->change_down == 1)
		gun_change_up(sdl, player,  frame);
}

void	keyboard_input(t_app *wolf, const Uint8 *key, float frame)
{
	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(wolf->player, key);
	if (key[SDL_SCANCODE_SPACE] && wolf->player->anim_frame == 0)
	{
		if (wolf->player->weapon[wolf->player->cur_weapon].ammo_cur > 0)
		{
			printf("SHOOT: START\n");
			wolf->player->anim_frame = frame;
			wolf->player->anim_is_done = 0;
			wolf->player->shooting = 1;
		}
	}
	if (key[SDL_SCANCODE_Q] && wolf->player->anim_frame == 0)
	{
		printf("CHANGE: START\n");
		wolf->player->anim_frame = frame;
		wolf->player->anim_is_done = 0;
		wolf->player->changing = 1;
	}
	if (key[SDL_SCANCODE_R] && wolf->player->anim_frame == 0)
	{
		if (wolf->player->weapon[wolf->player->cur_weapon].ammo_cur < wolf->player->weapon[wolf->player->cur_weapon].ammo_max
			&& wolf->player->weapon[wolf->player->cur_weapon].mag_cur > 0)
		{
			printf("RELOAD: START\n");
			wolf->player->anim_frame = frame;
			wolf->player->anim_is_done = 0;
			wolf->player->reloading = 1;
		}
	}
	player_movement(wolf->map, key, wolf->player);
	update_sound(key, wolf->player);
}
