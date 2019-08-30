/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:25:35 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 13:08:23 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	load_sounds(t_app *app, t_player *player)
{
	load_sound(app, &player->fx_empty, "GunEmpty.wav");
	load_sound(app, &player->fx_reload, "GunReload.wav");
	load_sound(app, &player->sound_effect, "speak.wav");
	load_sound(app, &player->steps[0], "step-01.wav");
	load_sound(app, &player->steps[1], "step-02.wav");
	load_sound(app, &player->steps[2], "step-03.wav");
	load_sound(app, &player->steps[3], "step-04.wav");
	load_sound(app, &player->steps[4], "step-05.wav");
	load_sound(app, &player->steps[5], "step-06.wav");
	load_sound(app, &player->steps[6], "step-07.wav");
	load_sound(app, &player->steps[7], "step-08.wav");
}

void	load_weapons(t_app *app, t_player *player)
{
	player->weapon = (t_weapon *)malloc(sizeof(t_weapon) * 3);
	init_weapon(&player->weapon[0], 2, 0.07f);
	init_weapon_sprite(app, &player->weapon[0], "hunter/");
	init_weapon(&player->weapon[1], 6, 0.09f);
	init_weapon_sprite(app, &player->weapon[1], "rustmag/");
	init_weapon(&player->weapon[2], 30, 0.02f);
	init_weapon_sprite(app, &player->weapon[2], "buzzsaw/");
}

void	load_faces(t_app *app, t_player *player)
{
	char folder[50];

	folder[0] = '\0';
	ft_strcat(folder, "./resources/player/faces/");
	player->faces[0] = load_texture(app, folder, "A0.bmp");
	player->faces[1] = load_texture(app, folder, "B0.bmp");
	player->faces[2] = player->faces[0];
	player->faces[3] = load_texture(app, folder, "D0.bmp");
	player->faces[4] = NULL;
}
