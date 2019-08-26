/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:25:29 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:25:29 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int	is_move_input(const Uint8 *key)
{
	if (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_UP])
		return (1);
	if (key[SDL_SCANCODE_A] || key[SDL_SCANCODE_LEFT])
		return (1);
	if (key[SDL_SCANCODE_S] || key[SDL_SCANCODE_DOWN])
		return (1);
	if (key[SDL_SCANCODE_D] || key[SDL_SCANCODE_RIGHT])
		return (1);
	return (0);
}

void		keyboard_input(t_app *app, float frame)
{
	const Uint8 *key;

	key = app->inputs->keyboard;
	if (is_move_input(key) && app->player->move_acc < 3.5)
		app->player->move_acc += 0.8;
	else if (!is_move_input(key) && app->player->move_acc > 0)
		app->player->move_acc -= 1.5;
	if (app->player->move_acc > 0)
		app->player->speed = 50.0 * app->player->move_acc * app->time->delta;
	if (app->player->speed > 0)
		player_movement(app->map->nodes, key, app->player);
	if (app->inputs->left_pressed)
		animation_start(&app->animations[ANIM_SHOOT]);
	if (key[SDL_SCANCODE_Q])
		animation_start(&app->animations[ANIM_CHANGE]);
	if (key[SDL_SCANCODE_R])
		animation_start(&app->animations[ANIM_RELOAD]);
	if (key[SDL_SCANCODE_SPACE] && frame - app->player->last_space >= 0.3)
		door_interaction(app, frame);
	if (key[SDL_SCANCODE_M] || key[SDL_SCANCODE_P])
		update_sound(key, app->sfx);
}
