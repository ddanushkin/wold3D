/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 11:21:57 by lglover           #+#    #+#             */
/*   Updated: 2019/06/03 17:28:19 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

SDL_Texture	*load_texture(t_sdl *sdl, char *name)
{
	char		file_path[50];
	SDL_Surface *surface;
	SDL_Texture	*texture;
	Uint32		key;

	ft_strcpy(file_path, "../resources/");
	ft_strcat(file_path, name);
	ft_strcat(file_path, ".bmp");
	surface = SDL_LoadBMP(file_path);
	key = SDL_MapRGB(surface->format, 152, 0, 136);
	SDL_SetColorKey(surface, SDL_TRUE, key);
	texture = SDL_CreateTextureFromSurface(sdl->renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}

static void	draw_ceiling(t_sdl *sdl, int x, int y)
{
	int			j;
	SDL_Color	color;
	double		dist;

	if (y > 0)
	{
		j = 0;
		while (j < y)
		{
			color.r = 31;
			color.g = 44;
			color.b = 61;
			dist = (sdl->height / (2.0 * j - sdl->height)) * 5;
			shade_color(dist, &color, -58);
			set_pixel(sdl, x, j, &color);
			j++;
		}
	}
}

static void	draw_floor(t_sdl *sdl, int x, int end)
{
	int			j;
	SDL_Color	color;
	double		dist;

	if (end < sdl->height)
	{
		j = end;
		while (j < sdl->height)
		{
			color.r = 21;
			color.g = 34;
			color.b = 51;
			dist = (sdl->height / (2.0 * j - sdl->height)) * 5;
			shade_color(dist, &color, 58);
			set_pixel(sdl, x, j, &color);
			j++;
		}
	}
}

int			draw_back(t_sdl *sdl, int y, int x, int end)
{
	int		offset;

	while (y < end)
	{
		offset = y * sdl->width + x;
		sdl->pixels[offset] = 0;
		y++;
	}
	return (y);
}

void		draw_column(t_sdl *sdl, t_ray *ray, int x, int height)
{
	int			y;
	int			begin;
	int			end;
	double		ratio;
	SDL_Color	color;

	ratio = 64.0 / height;
	begin = (sdl->height - height) / 2;
	if ((end = begin + height) > sdl->height)
		end = sdl->height;
	y = (begin < 0) ? 0 : begin;
	draw_ceiling(sdl, x, y);
	draw_floor(sdl, x, end);
	if (ray->dist > sdl->draw_dist)
		y = draw_back(sdl, y, x, end);
	while (y < end)
	{
		get_color(ray->texture, &color, ray->offset, (y - begin) * ratio);
		shade_color(ray->dist, &color, sdl->draw_dist);
		set_pixel(sdl, x, y, &color);
		y++;
	}
}
