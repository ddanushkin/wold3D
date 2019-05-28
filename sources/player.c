/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 17:57:04 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/28 13:26:24 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	player_init(t_sdl *sdl, t_player *player)
{
	t_weapon *weapon;

	weapon = (t_weapon *)malloc(sizeof(t_weapon));
	player->direction = 90;
	player->lives = 99;
	player->health = 100;
	player->score = 0;
	player->cur_level = 0;
	player->bgm = Mix_LoadMUS("../resources/bgm.mp3");
	player->sound_effect = Mix_LoadWAV("../resources/speak.wav");
	player->state[0] = load_texture(sdl, "fpain L1");
	player->state[1] = load_texture(sdl, "fpain R1");
	weapon->ammo = 10;
	weapon->firerate = 1;
	weapon->sprites[0] = load_texture(sdl, "Hunterfloor");
	weapon->sprites[1] = load_texture(sdl, "Hunter1");
	weapon->sprites[2] = load_texture(sdl, "Hunter2");
	weapon->sprites[3] = load_texture(sdl, "Hunter3");
	weapon->sprites[4] = load_texture(sdl, "Hunter4");
	weapon->sprites[5] = load_texture(sdl, "Hunter5");
	weapon->sprites[6] = load_texture(sdl, "Hunter6");
	weapon->sprites[7] = load_texture(sdl, "Hunter7");
	weapon->sprites[8] = load_texture(sdl, "Hunter8");
	weapon->sprites[9] = load_texture(sdl, "Hunter9");
	weapon->sprites[10] = load_texture(sdl, "Hunter10");
	player->shooting = 0;
	player->weapon = weapon;
}

void	draw_face(t_sdl *sdl, t_player *player, float *delta)
{
	SDL_Rect	area;
	int			cur_frame;

	area.y = sdl->height - 130;
	area.w = 96;
	area.x = sdl->width / 2 - area.w + 5;
	area.h = 116;

	cur_frame = (long)*delta % 2;
	SDL_RenderCopy(sdl->renderer, player->state[cur_frame], NULL, &area);
}

void	gun_idle(t_sdl *sdl, t_player *player, float *delta)
{
	SDL_Rect	area;
	int			cur_frame;

	cur_frame = (long)(*delta * 1.9);
	area.y = sdl->height - 130 - 550 + (cur_frame % 2 * 5);
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;

	cur_frame %= 4;
	if (cur_frame == 0)
		cur_frame++;
	SDL_RenderCopy(sdl->renderer, player->weapon->sprites[cur_frame], NULL, &area);
}

void	gun_shoot(t_sdl *sdl, t_player *player, float *delta)
{
	SDL_Rect	area;
	int			cur_frame;

	cur_frame = (long)((*delta - player->shooting) * 8);
	area.y = sdl->height - 130 - 550 + (cur_frame % 2 * 5);
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	cur_frame %= 11;
	if (cur_frame + 4 < 11)
		SDL_RenderCopy(sdl->renderer, player->weapon->sprites[cur_frame + 4], NULL, &area);
	else
		SDL_RenderCopy(sdl->renderer, player->weapon->sprites[2], NULL, &area);
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

void	player_shoot(t_map *map, const Uint8 *key, t_player *player, float frame)
{
		if (player->shooting == 0)
		{
			player->weapon->ammo--;
			player->shooting = frame;
		}
}

void	keyboard_input(t_map *map, const Uint8 *key, t_player *player, float frame)
{
	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(player, key);
	if (key[SDL_SCANCODE_SPACE] && player->weapon->ammo)
		player_shoot(map, key, player, frame);
	if (frame - player->shooting >= player->weapon->firerate)
		player->shooting = 0;
	player_movement(map, key, player);
	update_sound(key, player);
}
