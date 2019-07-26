#include "wolf3d.h"

void		load_sounds(t_player *player)
{
	player->fx_die = Mix_LoadWAV("../resources/sounds/Player Dies.wav");
	player->fx_empty = Mix_LoadWAV("../resources/sounds/GunEmpty.wav");
	player->fx_reload = Mix_LoadWAV("../resources/sounds/GunReload.wav");
	player->sound_effect = Mix_LoadWAV("../resources/sounds/speak.wav");
	player->steps[0] = Mix_LoadWAV("../resources/sounds/step-01.wav");
	player->steps[1] = Mix_LoadWAV("../resources/sounds/step-02.wav");
	player->steps[2] = Mix_LoadWAV("../resources/sounds/step-03.wav");
	player->steps[3] = Mix_LoadWAV("../resources/sounds/step-04.wav");
	player->steps[4] = Mix_LoadWAV("../resources/sounds/step-05.wav");
	player->steps[5] = Mix_LoadWAV("../resources/sounds/step-06.wav");
	player->steps[6] = Mix_LoadWAV("../resources/sounds/step-07.wav");
	player->steps[7] = Mix_LoadWAV("../resources/sounds/step-08.wav");
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

	area.h = 116;
	area.w = 96;
	area.x = sdl->half_width - area.w + 5;
	area.y = sdl->height - 130;
	cur_frame = (long)delta % 4 * 7;
	if (cur_frame == 14)
		cur_frame = 0;
	if (player->health < 85)
		cur_frame += 6 - ((player->health) / 14) % 7;
	SDL_RenderCopy(sdl->renderer, player->faces[cur_frame], NULL, &area);
}

void		load_faces(t_sdl *sdl, t_player *player)
{
	load_textures(sdl, player->faces, "../resources/player/faces/");
}

void		player_init(t_sdl *sdl, t_player *player)
{
	ft_bzero(player, sizeof(t_player));
	player->anim_is_done = 1;
	player->direction = 90;
	player->lives = 99;
	player->health = 100;
	player->max_dist = 25;
	player->x_v = cos(player->direction * M_PI_180);
	player->y_v = sin(player->direction * M_PI_180);
	load_faces(sdl, player);
	load_sounds(player);
	load_weapons(sdl, player);
}

void		get_weapon_sprites(t_sdl *sdl, t_weapon *weapon, char *gun_path)
{
	char	folder_path[50];

	ft_strcpy(folder_path, "../resources/weapons/");
	ft_strcat(folder_path, gun_path);
	load_textures(sdl, weapon->sprites, folder_path);
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

void		update_sound(const Uint8 *key, t_sfx *sfx)
{
	if (key[SDL_SCANCODE_M])
	{
		if (!Mix_PlayingMusic())
			Mix_PlayMusic(sfx->background, -1);
		else if (Mix_PausedMusic())
			Mix_ResumeMusic();
	}
	if (key[SDL_SCANCODE_P])
		Mix_PauseMusic();
}
