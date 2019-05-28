/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 17:57:04 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/28 19:42:32 by lglover          ###   ########.fr       */
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

	player->anim_is_done = 1;

	player->weapon[0].ammo = 10;
	player->weapon[0].firerate = 1;
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

	player->weapon[1].ammo = 6;
	player->weapon[1].firerate = 2;
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

	player->shooting = 0;
	player->reloading = 0;
	player->cur_weapon = 0;
	player->change_down = 0;
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

	cur_frame %= 4;
	if (cur_frame == 0)
		cur_frame++;
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[cur_frame], NULL, &area);
}

void	player_shoot(t_map *map, const Uint8 *key, t_player *player, float frame)
{
		printf("anim shoot - %d\n", player->anim_is_done);
		if (player->weapon[player->cur_weapon].ammo == 0)
			return ;
		if (player->shooting == 0)
		{
			if (player->weapon[player->cur_weapon].ammo > 0)
				player->weapon[player->cur_weapon].ammo--;
			player->shooting = frame;
		}
}

void	gun_shoot(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;
	unsigned char	id;

	id = player->cur_weapon;
	cur_frame = (long)((delta - player->shooting) * 8);
	area.y = sdl->height - 130 - 550 + (cur_frame % 2 * 5);
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	cur_frame %= 11;
	if (cur_frame + 4 < 11)
		SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[cur_frame + 4], NULL, &area);
	else
	{
		SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[2], NULL, &area);
		player->anim_is_done = 1;
	}
}

void 	player_change(t_player *player, float frame)
{
	printf("anim shoot - %d\n", player->anim_is_done);
	if (player->reloading == 0)
		player->reloading = frame;
}

void 	gun_change_down(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;
	unsigned char	id;

	id = player->cur_weapon;
	cur_frame = (long)((delta - player->reloading) * 32);
	area.y = sdl->height - 130 - 550 + 4 * (cur_frame % 100);
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[2], NULL, &area);
	if (area.y > 220)
	{
		player->change_down = 1;
		player->cur_weapon = player->cur_weapon ? 0 : 1;
	}
}

void 	gun_change_up(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;
	unsigned char	id;

	id = player->cur_weapon;
	cur_frame = (long)((delta - player->reloading) * 32);
	area.y = sdl->height - 130 - 150 - 4 * (cur_frame % 100);
	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[2], NULL, &area);
	if (area.y >= 40)
	{
		player->change_down = 2;
		player->anim_is_done = 1;
	}
}

void 	gun_change(t_sdl *sdl, t_player *player, unsigned int next_weapon, float delta)
{
	if (player->change_down == 0)
		gun_change_down(sdl, player, delta);
	if (player->change_down == 1)
		gun_change_up(sdl, player,  delta);
}

void	keyboard_input(t_app *wolf, const Uint8 *key, float frame)
{
	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(wolf->player, key);
	if (key[SDL_SCANCODE_SPACE] && wolf->player->anim_is_done == 1)
		player_shoot(wolf->map, key, wolf->player, frame);
	else if (key[SDL_SCANCODE_Q] && wolf->player->anim_is_done == 1)
		player_change(wolf->player, frame);
	player_movement(wolf->map, key, wolf->player);
	update_sound(key, wolf->player);
}
