/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:51:10 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/24 11:50:35 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	get_color(SDL_Surface *surface, SDL_Color *c, int x, int y)
{
	Uint8	*data;

	data = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
	SDL_GetRGB(*data, surface->format, &c->r, &c->g, &c->b);
}

void	shade_color(double dist, SDL_Color *color, double shade_dist)
{
	double	factor;

	factor = dist / shade_dist;
	if (factor > 1)
	{
		color->r = 0;
		color->g = 0;
		color->b = 0;
	}
	else
	{
		color->r = color->r * (1 - factor);
		color->g = color->g * (1 - factor);
		color->b = color->b * (1 - factor);
	}
}

void	set_pixel(t_sdl *sdl, int x, int y, SDL_Color *color)
{
	int		offset;

	offset = y * sdl->width + x;
	sdl->pixels[offset] = (Uint32)((color->r << 16) + (color->g << 8) + (color->b));
}