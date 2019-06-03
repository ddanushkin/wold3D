/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 17:57:04 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/03 21:44:45 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		load_sounds(t_player *player)
{
	player->bgm = Mix_LoadMUS("../resources/sounds/bgm.mp3");
	player->sound_effect = Mix_LoadWAV("../resources/sounds/speak.wav");
	player->fx_reload = Mix_LoadWAV("../resources/sounds/GunReload.wav");
	player->fx_empty = Mix_LoadWAV("../resources/sounds/GunEmpty.wav");
}

void		load_weapons(t_sdl *sdl, t_player *player)
{
	player->weapon = (t_weapon *)malloc(sizeof(t_weapon) * 3);
	get_weapon_sprites(sdl, &player->weapon[0], "hunter/");
	init_weapon(&player->weapon[0], 10, 0.8, "Huntershoot.wav");
	get_weapon_sprites(sdl, &player->weapon[1], "rustmag/");
	init_weapon(&player->weapon[1], 6, 1.7, "Rustmagshoot.wav");
	get_weapon_sprites(sdl, &player->weapon[2], "buzzsaw/");
	init_weapon(&player->weapon[2], 30, 0.8, "Buzzsawshoot.wav");
}

void		load_faces(t_sdl *sdl, SDL_Texture *state[])
{
	state[0] = load_texture(sdl, "/faces/fpain L1");
	state[1] = load_texture(sdl, "/faces/fpain F1");
	state[2] = load_texture(sdl, "/faces/fpain R1");
	state[3] = load_texture(sdl, "/faces/fpain F1");
	state[4] = load_texture(sdl, "/faces/fpain L4");
	state[5] = load_texture(sdl, "/faces/fpain F4");
	state[6] = load_texture(sdl, "/faces/fpain R4");
	state[7] = load_texture(sdl, "/faces/fpain F4");
}

void		player_init(t_sdl *sdl, t_player *player)
{
	ft_bzero(player, sizeof(t_player));
	player->direction = 90;
	player->lives = 99;
	player->health = 50;
	player->cur_weapon = 2;
	player->anim_is_done = 1;
	load_sounds(player);
	load_faces(sdl, player->state);
	load_weapons(sdl, player);
}

void		draw_face(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;

	area.y = sdl->height - 130;
	area.w = 96;
	area.x = sdl->width / 2 - area.w + 5;
	area.h = 116;
	cur_frame = (long)delta % 4;
	if (player->health >= 60)
		SDL_RenderCopy(sdl->renderer, player->state[cur_frame], NULL, &area);
	else
		SDL_RenderCopy(sdl->renderer, player->state[cur_frame + 4], NULL, &area);
}

SDL_Texture	*load_sprite(t_sdl *sdl, char *folder_path, char *sprite_name)
{
	char			file_path[50];
	Uint32			key;
	SDL_Texture		*texture;
	SDL_Surface		*surface;

	ft_strcpy(file_path, folder_path);
	ft_strcat(file_path, sprite_name);
	surface = SDL_LoadBMP(file_path);
	key = SDL_MapRGB(surface->format, 152, 0, 136);
	SDL_SetColorKey(surface, SDL_TRUE, key);
	texture = SDL_CreateTextureFromSurface(sdl->renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}

void		get_sprites(t_sdl *sdl, t_weapon *weapon, char *path)
{
	DIR				*d;
	struct dirent	*dir;
	u_int			i;

	i = 0;
	d = opendir(path);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (ft_strequ(".", dir->d_name) || ft_strequ("..", dir->d_name))
				continue ;
			else
				weapon->sprites[i++] = load_sprite(sdl, path, dir->d_name);
		}
		closedir(d);
	}
}

void		get_weapon_sprites(t_sdl *sdl, t_weapon *weapon, char *weapon_folder)
{
	char	folder_path[50];

	ft_strcpy(folder_path, "../resources/weapons/");
	ft_strcat(folder_path, weapon_folder);
	get_sprites(sdl, weapon, folder_path);
}

void		init_weapon(t_weapon *weapon, u_int ammo, float rate, char *sound)
{
	char	file_path[50];

	weapon->ammo_cur = ammo;
	weapon->ammo_max = ammo;
	weapon->mag_cur = ammo * 4;
	weapon->firerate = rate;
	weapon->fired = 0;
	ft_strcpy(file_path, "../resources/sounds/");
	ft_strcat(file_path, sound);
	weapon->gun_sound = Mix_LoadWAV(file_path);
}

void		player_rotate(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		(player->direction -= player->speed * 0.5) < 0 ? player->direction = 359 : 0;
	if (state[SDL_SCANCODE_RIGHT])
		(player->direction += player->speed * 0.5) > 359 ? player->direction = 1 : 0;
}

float		get_ray_length(t_map *map, t_player *player, double angle)
{
	float	dist;
	t_ray	*ray;

	angle = (long)(angle) % 360;
	ray = get_ray(map, player, angle);
	dist = ray->dist;
	free(ray);
	return (dist / cos(fabs(angle - player->direction) * M_PI_180));
}

void		player_update_dist(t_map *map, t_player *player)
{
	player->dist_n = get_ray_length(map, player, player->direction);
	player->dist_s = get_ray_length(map, player, player->direction - 180);
	player->dist_e = get_ray_length(map, player, player->direction - 90);
	player->dist_w = get_ray_length(map, player, player->direction + 90);
}

void		player_movement(t_map *map, const Uint8 *key, t_player *player)
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

void		update_sound(const Uint8 *key, t_player *player)
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
}
