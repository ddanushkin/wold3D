/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:21:51 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 13:07:09 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	state_change(t_app *app)
{
	if (app->player->state == PL_STATE_IDLE)
	{
		if (app->animations[ANIM_SHOOT].play)
			app->player->state = PL_STATE_SHOOT;
		else if (app->animations[ANIM_CHANGE].play)
			app->player->state = PL_STATE_CHANGE;
		else if (app->animations[ANIM_RELOAD].play)
			app->player->state = PL_STATE_RELOAD;
	}
	if (app->player->state != PL_STATE_IDLE)
		app->animations[ANIM_IDLE].play = 0;
}

void	animation_idle_weapon(t_app *app)
{
	animation_start(&app->animations[ANIM_IDLE]);
	animation_next_frame(&app->animations[ANIM_IDLE]);
	idle_draw(app, &app->animations[ANIM_IDLE]);
}

void	animation_change_weapon(t_app *app)
{
	animation_next_frame(&app->animations[ANIM_CHANGE]);
	change_draw(app, &app->animations[ANIM_CHANGE]);
	if (animation_ended(app, &app->animations[ANIM_CHANGE]))
		app->player->changed = 0;
}

void	animation_reload_weapon(t_app *app)
{
	animation_next_frame(&app->animations[ANIM_RELOAD]);
	reload_draw(app, &app->animations[ANIM_RELOAD]);
	if (animation_ended(app, &app->animations[ANIM_RELOAD]))
		app->player->reloaded = 0;
}

void	animation_shoot_weapon(t_app *app)
{
	t_weapon		*weapon;

	weapon = &app->player->weapon[app->player->cur_weapon];
	app->animations[ANIM_SHOOT].speed = weapon->firerate;
	animation_next_frame(&app->animations[ANIM_SHOOT]);
	shoot_draw(app, &app->animations[ANIM_SHOOT]);
	if (animation_ended(app, &app->animations[ANIM_SHOOT]))
		app->player->weapon[app->player->cur_weapon].fired = 0;
}
