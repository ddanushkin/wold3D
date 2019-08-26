/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_ui.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:24:58 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:24:58 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		update_level(SDL_Renderer *renderer, char *level,
							t_ui_elem *ui_elem, TTF_Font *font)
{
	ui_elem->x = 90;
	ui_elem->text = level;
	draw_text_font(renderer, ui_elem, font);
}

void		update_score(SDL_Renderer *renderer, int score, t_ui_elem *ui_elem,
							TTF_Font *font)
{
	char	*ui_text;

	ui_elem->x = 150 + 40;
	ui_text = ft_itoa(score);
	ui_elem->text = ui_text;
	draw_text_font(renderer, ui_elem, font);
	free(ui_text);
}

void		update_life(SDL_Renderer *renderer, int lives, t_ui_elem *ui_elem,
							TTF_Font *font)
{
	char	*ui_text;

	ui_elem->x = 150 + 40 + 235;
	ui_text = ft_itoa(lives);
	ui_elem->text = ui_text;
	draw_text_font(renderer, ui_elem, font);
	free(ui_text);
}

void		update_health(SDL_Renderer *renderer, int health,
							t_ui_elem *ui_elem, TTF_Font *font)
{
	char	*ui_text;

	ui_elem->x = 150 + 40 + 235 + 260;
	ui_text = ft_itoa(health);
	ui_elem->text = ui_text;
	draw_text_font(renderer, ui_elem, font);
	free(ui_text);
}

void		update_ammo(SDL_Renderer *renderer, t_player *player,
							t_ui_elem *ui_elem, TTF_Font *font)
{
	char	ammo_text[50];
	char	*ammo;
	char	*mag;

	ui_elem->x = 150 + 40 + 235 + 260 + 170;
	ammo_text[0] = '\0';
	ammo = ft_itoa(player->weapon[player->cur_weapon].ammo_cur);
	mag = ft_itoa(player->weapon[player->cur_weapon].mag_cur);
	ft_strcat(ammo_text, ammo);
	ft_strcat(ammo_text, " | ");
	ft_strcat(ammo_text, mag);
	ft_strdel(&ammo);
	ft_strdel(&mag);
	ui_elem->text = ammo_text;
	draw_text_font(renderer, ui_elem, font);
}
