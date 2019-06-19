/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 17:57:04 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/19 10:30:43 by lglover          ###   ########.fr       */
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

void		draw_face(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;

	area.y = sdl->height - 130;
	area.w = 96;
	area.x = sdl->width / 2 - area.w + 5;
	area.h = 116;
	cur_frame = (long)delta % 4 * 7;
	if (cur_frame == 14)
		cur_frame = 0;
	if (player->health < 85)
		cur_frame += 6 - ((player->health) / 14) % 7;
	SDL_RenderCopy(sdl->renderer, player->state[cur_frame], NULL, &area);
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

void		get_sprites(t_sdl *sdl, SDL_Texture *sprites[], char *path)
{
	DIR				*d;
	struct dirent	*dir;
	u_int			i;

	i = 0;
	printf("Load from: %s\n", path);
	d = opendir(path);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_name[0] == '.')
				continue ;
			else
			{
				sprites[i++] = load_sprite(sdl, path, dir->d_name);
				printf("Sprite loaded: %s\n", dir->d_name);
			}
		}
		closedir(d);
	}
}

void		load_faces(t_sdl *sdl, t_player *player)
{
	get_sprites(sdl, player->state, "../resources/player/faces/");
}

void		player_init(t_sdl *sdl, t_player *player)
{
	ft_bzero(player, sizeof(t_player));
	player->direction = 90;
	player->x_v = cos(player->direction * M_PI_180);
	player->y_v = sin(player->direction * M_PI_180);
	player->lives = 99;
	player->health = 100;
	player->cur_weapon = 0;
	player->anim_is_done = 1;
	player->max_dist = 25;
	load_sounds(player);
	load_faces(sdl, player);
	load_weapons(sdl, player);
}

void		get_weapon_sprites(t_sdl *sdl, t_weapon *weapon, char *weapon_folder)
{
	char	folder_path[50];

	ft_strcpy(folder_path, "../resources/weapons/");
	ft_strcat(folder_path, weapon_folder);
	get_sprites(sdl, weapon->sprites, folder_path);
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
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT])
	{
		player->x_v = cos(player->direction * M_PI_180);
		player->y_v = sin(player->direction * M_PI_180);
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
