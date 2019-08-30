/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 17:19:24 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 17:19:24 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			load_walls_1(t_app *app, t_textures *textures)
{
	textures->walls[0] = load_surface(app, "./resources/walls/", "1_0.bmp");
	textures->walls[1] = load_surface(app, "./resources/walls/", "1_1.bmp");
	textures->walls[2] = load_surface(app, "./resources/walls/", "1_2.bmp");
	textures->walls[3] = load_surface(app, "./resources/walls/", "1_3.bmp");
	textures->walls[4] = load_surface(app, "./resources/walls/", "2_0.bmp");
	textures->walls[5] = load_surface(app, "./resources/walls/", "2_1.bmp");
	textures->walls[6] = load_surface(app, "./resources/walls/", "2_2.bmp");
	textures->walls[7] = load_surface(app, "./resources/walls/", "2_3.bmp");
	textures->walls[8] = load_surface(app, "./resources/walls/", "3_0.bmp");
	textures->walls[9] = load_surface(app, "./resources/walls/", "3_1.bmp");
	textures->walls[10] = load_surface(app, "./resources/walls/", "3_2.bmp");
	textures->walls[11] = load_surface(app, "./resources/walls/", "3_3.bmp");
	textures->walls[12] = load_surface(app, "./resources/walls/", "4_0.bmp");
	textures->walls[13] = load_surface(app, "./resources/walls/", "4_1.bmp");
	textures->walls[14] = load_surface(app, "./resources/walls/", "4_2.bmp");
	textures->walls[15] = load_surface(app, "./resources/walls/", "4_3.bmp");
	textures->walls[16] = load_surface(app, "./resources/walls/", "5_0.bmp");
	textures->walls[17] = load_surface(app, "./resources/walls/", "5_1.bmp");
	textures->walls[18] = load_surface(app, "./resources/walls/", "5_2.bmp");
	textures->walls[19] = load_surface(app, "./resources/walls/", "5_3.bmp");
	textures->walls[20] = load_surface(app, "./resources/walls/", "6_0.bmp");
	textures->walls[21] = load_surface(app, "./resources/walls/", "6_1.bmp");
	textures->walls[22] = load_surface(app, "./resources/walls/", "6_2.bmp");
	textures->walls[23] = load_surface(app, "./resources/walls/", "6_3.bmp");
}

void			load_walls_2(t_app *app, t_textures *textures)
{
	textures->walls[24] = load_surface(app, "./resources/walls/", "7_0.bmp");
	textures->walls[25] = load_surface(app, "./resources/walls/", "7_1.bmp");
	textures->walls[26] = load_surface(app, "./resources/walls/", "7_2.bmp");
	textures->walls[27] = load_surface(app, "./resources/walls/", "7_3.bmp");
	textures->walls[28] = load_surface(app, "./resources/walls/", "8_0.bmp");
	textures->walls[29] = load_surface(app, "./resources/walls/", "8_1.bmp");
	textures->walls[30] = load_surface(app, "./resources/walls/", "8_2.bmp");
	textures->walls[31] = load_surface(app, "./resources/walls/", "8_3.bmp");
	textures->walls[32] = load_surface(app, "./resources/walls/", "9_0.bmp");
	textures->walls[33] = load_surface(app, "./resources/walls/", "9_1.bmp");
	textures->walls[34] = load_surface(app, "./resources/walls/", "9_2.bmp");
	textures->walls[35] = load_surface(app, "./resources/walls/", "9_3.bmp");
}