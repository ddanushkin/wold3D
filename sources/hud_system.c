/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_system.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:23:57 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 18:51:58 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		load_font(t_app *app, int size)
{
	app->sdl->font = TTF_OpenFont("./resources/ui_font.ttf", size);
	if (app->sdl->font == NULL)
		app->error = 1;
}

void		draw_text_font(SDL_Renderer *renderer, t_ui_elem *ui_elem,
		TTF_Font *font)
{
	SDL_Surface		*text_surface;
	SDL_Texture		*text_texture;
	SDL_Rect		text_area;
	SDL_Color		color;

	color.r = 255;
	color.g = 77;
	color.b = 255;
	color.a = 255;
	text_surface = TTF_RenderText_Solid(font, ui_elem->text, color);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	text_area.x = ui_elem->x;
	text_area.y = ui_elem->y;
	text_area.w = text_surface->w;
	text_area.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_area);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void		create_hud(t_sdl *sdl, t_player *player)
{
	SDL_Rect	area;
	t_ui_elem	ui_elem;

	area.y = sdl->height - sdl->height / 5;
	area.x = 0;
	area.w = sdl->width;
	area.h = sdl->height / 5;
	ui_elem.y = sdl->height - 105;
	ui_elem.size = 60;
	SDL_RenderCopy(sdl->renderer, sdl->ui, NULL, &area);
	update_level(sdl->renderer, player->cur_level, &ui_elem, sdl->font);
	update_score(sdl->renderer, player->score, &ui_elem, sdl->font);
	update_life(sdl->renderer, player->lives, &ui_elem, sdl->font);
	update_health(sdl->renderer, player->health, &ui_elem, sdl->font);
	update_ammo(sdl->renderer, player, &ui_elem, sdl->font);
}
