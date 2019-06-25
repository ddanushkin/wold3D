/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 15:57:40 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/25 22:00:20 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

TTF_Font	*load_font(int size)
{
	TTF_Font *font;

	font = TTF_OpenFont("../resources/ui_font.ttf", size);
	return (font);
}

void		draw_text(SDL_Renderer *renderer, t_ui_elem *ui_elem)
{
	SDL_Surface		*text_surface;
	SDL_Texture		*text_texture;
	SDL_Rect		text_area;
	SDL_Color		color;
	TTF_Font		*font;

	font = load_font(ui_elem->size);
	color.r = 200;
	color.g = 77;
	color.b = 255;
	color.a = 255;
	text_surface = TTF_RenderText_Solid(font, ui_elem->text, color);
	text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);
	text_area.x = ui_elem->x;
	text_area.y = ui_elem->y;
	text_area.w = text_surface->w;
	text_area.h = text_surface->h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_area);
	SDL_DestroyTexture(text_texture);
	TTF_CloseFont(font);
}

void		update_level(SDL_Renderer *renderer, char *level, t_ui_elem *ui_elem)
{
	ui_elem->x = 90;
	ui_elem->text = level;
	draw_text(renderer, ui_elem);
}

void		update_score(SDL_Renderer *renderer, int score, t_ui_elem *ui_elem)
{
	char	*ui_text;

	ui_elem->x = 150 + 40;
	ui_text = ft_itoa(score);
	ui_elem->text = ui_text;
	draw_text(renderer, ui_elem);
	free(ui_text);
}

void		update_lives(SDL_Renderer *renderer, int lives, t_ui_elem *ui_elem)
{
	char	*ui_text;

	ui_elem->x = 150 + 40 + 235;
	ui_text = ft_itoa(lives);
	ui_elem->text = ui_text;
	draw_text(renderer, ui_elem);
	free(ui_text);
}

void		update_health(SDL_Renderer *renderer, int health, t_ui_elem *ui_elem)
{
	char	*ui_text;

	ui_elem->x = 150 + 40 + 235 + 260;
	ui_text = ft_itoa(health);
	ui_elem->text = ui_text;
	draw_text(renderer, ui_elem);
	free(ui_text);
}

void		update_ammo(SDL_Renderer *renderer, t_player *player, t_ui_elem *ui_elem)
{
	char	ammo_text[50];
	char 	*divider;
	char	*ui_text;

	ui_elem->x = 150 + 40 + 235 + 260 + 170;
	ammo_text[0] = '\0';
	ui_text = ft_itoa(player->weapon[player->cur_weapon].ammo_cur);
	ft_strcat(ammo_text, ui_text);
	free(ui_text);
	ui_elem->text = ammo_text;
	draw_text(renderer, ui_elem);
	ui_elem->x = 150 + 40 + 235 + 260 + 170 + 90;
	divider = " | ";
	ui_elem->text = divider;
	draw_text(renderer, ui_elem);
}

void		update_ammo2(SDL_Renderer *renderer, t_player *player, t_ui_elem *ui_elem)
{
	char	ammo_text[50];
	char	*ui_text;

	ui_elem->x = 150 + 40 + 235 + 260 + 170 + 90 + 80;
	ammo_text[0] = '\0';
	ui_text = ft_itoa(player->weapon[player->cur_weapon].mag_cur);
	ft_strcat(ammo_text, ui_text);
	free(ui_text);
	ui_elem->text = ammo_text;
	draw_text(renderer, ui_elem);
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
	update_level(sdl->renderer, player->cur_level, &ui_elem);
	update_score(sdl->renderer, player->score, &ui_elem);
	update_lives(sdl->renderer, player->lives, &ui_elem);
	update_health(sdl->renderer, player->health, &ui_elem);
	update_ammo(sdl->renderer, player, &ui_elem);
	update_ammo2(sdl->renderer, player, &ui_elem);
}
