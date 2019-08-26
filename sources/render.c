/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:47 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:26:47 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_view(t_app *app)
{
	SDL_Rect		screen;

	if (app->inputs->right_pressed && app->offset < app->inputs->zoom)
		app->offset += 35;
	if (!app->inputs->right_pressed && app->offset > 50)
		app->offset -= 45;
	screen.x = -app->offset * 0.5 * 1.76;
	screen.y = -app->offset * 0.5 + app->player->head_offset;
	screen.w = app->sdl->width + app->offset * 1.76;
	screen.h = app->sdl->height + app->offset;
	SDL_UpdateTexture(app->sdl->texture, NULL, app->sdl->pixels,
			app->sdl->width * sizeof(Uint32));
	SDL_RenderCopy(app->sdl->renderer, app->sdl->texture, NULL, &screen);
}

void	redraw(t_app *app, float frame)
{
	if (app->player->state == PL_STATE_IDLE)
		animation_idle_weapon(app);
	if (app->player->state == PL_STATE_CHANGE)
		animation_change_weapon(app);
	if (app->player->state == PL_STATE_RELOAD)
		animation_reload_weapon(app);
	if (app->player->state == PL_STATE_SHOOT)
		animation_shoot_weapon(app);
	create_hud(app->sdl, app->player);
	draw_face(app->sdl, app->player, frame);
}
