#include "wolf3d.h"

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

void		change_weapon(t_app *app)
{
	app->player->cur_weapon++;
	if (app->player->cur_weapon > 2)
		app->player->cur_weapon = 0;
	app->player->changed = 1;
}

void		reload_weapon(t_app *app)
{
	int			old_ammo;
	t_weapon	*weapon;

	weapon = &app->player->weapon[app->player->cur_weapon];
	old_ammo = weapon->ammo_cur;
	if (weapon->mag_cur >= weapon->ammo_max)
	{
		weapon->ammo_cur = weapon->ammo_max;
		weapon->mag_cur -= weapon->ammo_max - old_ammo;
	}
	else
	{
		weapon->ammo_cur = weapon->mag_cur;
		weapon->mag_cur = 0;
	}
	app->player->reloaded = 1;
}
