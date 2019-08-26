/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:33 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 18:17:20 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		draw_face(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;

	area.h = 116;
	area.w = 96;
	area.x = sdl->half_width - area.w + 5;
	area.y = sdl->height - 130;
	cur_frame = (long)delta % 4;
	SDL_RenderCopy(sdl->renderer, player->faces[cur_frame], NULL, &area);
}

void		player_init(t_app *app, t_player *player)
{
	ft_bzero(player, sizeof(t_player));
	player->direction = 270;
	player->lives = 99;
	player->health = 100;
	player->max_dist = 25;
	player->x_v = cos(player->direction * M_PI_180);
	player->y_v = sin(player->direction * M_PI_180);
	load_faces(app, player);
	load_sounds(app, player);
	load_weapons(app, player);
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

void		place_player(t_app *app, t_node *node, int row, int col)
{
	app->player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
	app->player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
	app->players_count++;
	map_type_empty(node);
}

void		fix_direction(t_app *app)
{
	if (app->player->direction > 359)
		app->player->direction = 0;
	if (app->player->direction < 0)
		app->player->direction = 359;
}
