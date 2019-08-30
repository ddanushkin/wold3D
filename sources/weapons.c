/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:58 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 14:38:52 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		load_weapon_sound(t_app *app, Mix_Chunk **sound, char *name)
{
	char		file_path[50];

	file_path[0] = '\0';
	ft_strcpy(file_path, "./resources/weapons/");
	ft_strcat(file_path, name);
	ft_strcat(file_path, "shoot.wav");
	*sound = Mix_LoadWAV(file_path);
	if (*sound == NULL)
		app->error = 1;
}

void			init_weapon(t_weapon *weapon, u_int ammo, float rate)
{
	weapon->ammo_cur = ammo;
	weapon->ammo_max = ammo;
	weapon->mag_cur = ammo * 4;
	weapon->firerate = rate;
	weapon->fired = 0;
}

void			init_weapon_sprite(t_app *app, t_weapon *weapon,
									char *weapon_name)
{
	char	folder[50];

	folder[0] = '\0';
	ft_strcat(folder, "./resources/weapons/");
	ft_strcat(folder, weapon_name);
	weapon->sprites[0] = load_texture(app, folder, "0.bmp");
	weapon->sprites[1] = load_texture(app, folder, "1.bmp");
	weapon->sprites[2] = load_texture(app, folder, "2.bmp");
	weapon->sprites[3] = load_texture(app, folder, "3.bmp");
	weapon->sprites[4] = load_texture(app, folder, "4.bmp");
	weapon->sprites[5] = load_texture(app, folder, "5.bmp");
	weapon->sprites[6] = load_texture(app, folder, "6.bmp");
	weapon->sprites[7] = load_texture(app, folder, "7.bmp");
	weapon->sprites[8] = load_texture(app, folder, "8.bmp");
	weapon->sprites[9] = load_texture(app, folder, "9.bmp");
	weapon->sprites[10] = load_texture(app, folder, "10.bmp");
	load_weapon_sound(app, &weapon->gun_sound, weapon_name);
}

void			change_weapon(t_app *app)
{
	app->player->cur_weapon++;
	if (app->player->cur_weapon > 2)
		app->player->cur_weapon = 0;
	app->player->changed = 1;
}

void			reload_weapon(t_app *app)
{
	int			old_ammo;
	t_weapon	*weapon;

	weapon = &app->player->weapon[app->player->cur_weapon];
	old_ammo = weapon->ammo_cur;
	if (weapon->mag_cur == 0)
		Mix_PlayChannel(1, app->player->fx_empty, 0);
	if (weapon->mag_cur >= weapon->ammo_max)
	{
		weapon->ammo_cur = weapon->ammo_max;
		weapon->mag_cur -= weapon->ammo_max - old_ammo;
		Mix_PlayChannel(1, app->player->fx_reload, 0);
	}
	else
	{
		weapon->ammo_cur = weapon->mag_cur;
		weapon->mag_cur = 0;
		Mix_PlayChannel(1, app->player->fx_reload, 0);
	}
	app->player->reloaded = 1;
}
