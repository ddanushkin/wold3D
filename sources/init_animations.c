/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_animations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:25:20 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 15:18:21 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	init_idle_anim(t_app *app, t_animation *anim)
{
	anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	anim->area->y = app->sdl->height - 130 - 550;
	anim->area->w = 96 * 5;
	anim->area->x = app->sdl->half_width - anim->area->w + 244;
	anim->area->h = 116 * 5;
	anim->play = 0;
	anim->delayed = 0;
	anim->current_frame = 0;
	anim->total_frames = 30;
	anim->loop = 1;
	anim->delay = 0;
	anim->counter = 0;
	anim->speed = 0.05f;
}

void	init_shoot_anim(t_app *app, t_animation *anim)
{
	anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	anim->area->y = app->sdl->height - 130 - 550;
	anim->area->w = 96 * 5;
	anim->area->x = app->sdl->half_width - anim->area->w + 244;
	anim->area->h = 116 * 5;
	anim->play = 0;
	anim->delayed = 0;
	anim->current_frame = 0;
	anim->counter = 0;
	anim->total_frames = 11;
	anim->loop = 0;
	anim->delay = 0;
	anim->speed = 0.05f;
}

void	init_change_anim(t_app *app, t_animation *anim)
{
	anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	anim->area->y = app->sdl->height - 130 - 550;
	anim->area->w = 96 * 5;
	anim->area->x = app->sdl->half_width - anim->area->w + 244;
	anim->area->h = 116 * 5;
	anim->play = 0;
	anim->delayed = 0;
	anim->current_frame = 0;
	anim->counter = 0;
	anim->total_frames = 30;
	anim->loop = 0;
	anim->delay = 0;
	anim->speed = 0.05f;
	app->player->changed = 0;
}

void	init_reload_anim(t_app *app, t_animation *anim)
{
	anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	anim->area->y = app->sdl->height - 130 - 550;
	anim->area->w = 96 * 5;
	anim->area->x = app->sdl->half_width - anim->area->w + 244;
	anim->area->h = 116 * 5;
	anim->play = 0;
	anim->delayed = 0;
	anim->current_frame = 0;
	anim->counter = 0;
	anim->total_frames = 30;
	anim->loop = 0;
	anim->delay = 0;
	anim->speed = 0.05f;
	app->player->reloaded = 0;
}
