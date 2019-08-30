/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_weapons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:22:58 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 15:17:29 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	idle_draw(t_app *app, t_animation *anim)
{
	SDL_Texture		*sprite;

	sprite = app->player->weapon[app->player->cur_weapon].sprites[1];
	app->player->head_offset = sinf((float)app->time->current * 0.001f);
	if (app->player->move_acc > 0 && app->player->step
		&& anim->current_frame >= anim->total_frames * 0.5f)
	{
		Mix_PlayChannel(1, app->player->steps[rand() % 8], 0);
		app->player->step = 0;
	}
	if ((anim->current_frame + 2) >= anim->total_frames - 1)
		app->player->step = 1;
	anim->area->y -= (int)(app->player->head_offset);
	SDL_RenderCopy(app->sdl->renderer, sprite, NULL, anim->area);
	anim->area->y += (int)(app->player->head_offset);
}

void	shoot_draw(t_app *app, t_animation *anim)
{
	SDL_Texture		*sprite;
	t_weapon		*weapon;

	weapon = &app->player->weapon[app->player->cur_weapon];
	if (anim->current_frame == 0)
		anim->current_frame++;
	sprite = weapon->sprites[anim->current_frame];
	if (anim->current_frame == anim->total_frames - 1)
	{
		app->player->last_shoot = 0;
		weapon->fired = 0;
	}
	if (anim->current_frame > 3 && anim->current_frame < 10 && !weapon->fired)
		shoot_weapon(app);
	if (!app->player->last_shoot)
		sprite = weapon->sprites[1];
	SDL_RenderCopy(app->sdl->renderer, sprite, NULL, anim->area);
}

void	change_draw(t_app *app, t_animation *anim)
{
	SDL_Texture		*sprite;
	t_weapon		*weapon;
	SDL_Rect		pos;

	pos.x = anim->area->x;
	pos.w = anim->area->w;
	pos.h = anim->area->h;
	pos.y = anim->area->y + anim->current_frame * 10;
	if (anim->current_frame > anim->total_frames * 0.5)
	{
		if (!app->player->changed)
			change_weapon(app);
		pos.y = 340 - anim->current_frame * 10;
	}
	weapon = &app->player->weapon[app->player->cur_weapon];
	sprite = weapon->sprites[1];
	SDL_RenderCopy(app->sdl->renderer, sprite, NULL, &pos);
}

void	reload_draw(t_app *app, t_animation *anim)
{
	SDL_Texture		*sprite;
	t_weapon		*weapon;
	SDL_Rect		pos;

	pos.x = anim->area->x;
	pos.w = anim->area->w;
	pos.h = anim->area->h;
	pos.y = anim->area->y + anim->current_frame * 10;
	if (anim->current_frame > anim->total_frames * 0.5)
	{
		if (!app->player->reloaded)
			reload_weapon(app);
		pos.y = 340 - anim->current_frame * 10;
	}
	weapon = &app->player->weapon[app->player->cur_weapon];
	sprite = weapon->sprites[1];
	SDL_RenderCopy(app->sdl->renderer, sprite, NULL, &pos);
}
