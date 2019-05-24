/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 17:57:04 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/24 19:30:34 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	player_init(t_sdl *sdl, t_player *player)
{
	player->direction = 90;
	player->lives = 99;
	player->health = 100;
	player->score = 0;
	player->cur_level = 0;
	player->ammo = 10;
	player->bgm = Mix_LoadMUS("../resources/bgm.mp3");
	player->sound_effect = Mix_LoadWAV("../resources/speak.wav");
	player->state_1 = load_texture(sdl, "fpain L1");
	player->state_2 = load_texture(sdl, "fpain R1");
}

void	draw_face(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect area;

	area.y = sdl->height - 130;
	area.w = 96;
	area.x = sdl->width / 2 - area.w + 5;
	area.h = 116;

	if (delta)
		SDL_RenderCopy(sdl->renderer, player->state_1, NULL, &area);
	else
		SDL_RenderCopy(sdl->renderer, player->state_2, NULL, &area);
}

void	player_rotate(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		(player->direction -= player->speed * 0.5) < 0 ? player->direction = 357 : 0;
	if (state[SDL_SCANCODE_RIGHT])
		(player->direction += player->speed * 0.5) > 359 ? player->direction = 4 : 0;
//	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT])
//	{
//		printf("player_dir - %d\n", player->direction);
//		printf("dist_n - %f\n", player->dist_n);
//		printf("dist_w - %f\n", player->dist_w);
//		printf("dist_s - %f\n", player->dist_s);
//		printf("dist_e - %f\n", player->dist_e);
//	}
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
	if (key[SDL_SCANCODE_SPACE])
		Mix_PlayChannel(-1, player->sound_effect, 0);
}

void	keyboard_input(t_map *map, const Uint8 *key, t_player *player)
{
	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(player, key);
	player_movement(map, key, player);
	update_sound(key, player);
}
