#include "wolf3d.h"

void	load_sounds(t_player *player)
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

void	load_weapons(t_sdl *sdl, t_player *player)
{
	player->weapon = (t_weapon *)malloc(sizeof(t_weapon) * 3);
	get_weapon_sprites(sdl, &player->weapon[0], "hunter/");
	init_weapon(&player->weapon[0], 10, 0.8, "Huntershoot.wav");
	get_weapon_sprites(sdl, &player->weapon[1], "rustmag/");
	init_weapon(&player->weapon[1], 6, 1.7, "Rustmagshoot.wav");
	get_weapon_sprites(sdl, &player->weapon[2], "buzzsaw/");
	init_weapon(&player->weapon[2], 30, 0.8, "Buzzsawshoot.wav");
}

void	load_faces(t_sdl *sdl, t_player *player)
{
	load_textures(sdl, player->faces, "../resources/player/faces/");
}
