/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:21:55 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:22:01 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	get_color(SDL_Surface *surface, t_color *c, int x, int y)
{
	Uint8		*data;
	SDL_Color	color;

	data = surface->pixels + y * TEXTURE_SIZE + x;
	color = surface->format->palette->colors[*data];
	c->r = color.r;
	c->g = color.g;
	c->b = color.b;
}

void	shade_color(double dist, t_color *color, double draw_dist)
{
	double	factor;

	factor = dist / draw_dist;
	if (factor > 1)
	{
		color->r = 0;
		color->g = 0;
		color->b = 0;
	}
	else
	{
		factor = 1 - factor;
		color->r *= factor;
		color->g *= factor;
		color->b *= factor;
	}
}

void	set_pixel(t_sdl *sdl, int x, int y, t_color *col)
{
	int		offset;

	offset = y * sdl->width + x;
	sdl->pixels[offset] = (Uint32)((col->r << 16) + (col->g << 8) + (col->b));
}
