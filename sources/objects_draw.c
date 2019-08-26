/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:27 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:26:27 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	draw_slice(t_app *app, t_node *obj, t_iiter x, t_iiter y)
{
	int			texture_x;
	int			texture_y;
	t_color		col;

	texture_x = (x.cur - x.min) * (TEXTURE_SIZE / obj->height);
	texture_y = (y.cur - y.min) * (TEXTURE_SIZE / obj->height);
	get_color(obj->texture[0], &col, texture_x, texture_y);
	if (!(col.r == 152 && col.g == 0 && col.b == 136))
	{
		shade_color(obj->dist + 150, &col, app->sdl->draw_dist);
		set_pixel(app->sdl, x.cur, y.cur, &col);
	}
}

void		draw_object(t_app *app, t_node *obj)
{
	t_iiter	x;
	t_iiter	y;

	x.min = obj->screen_x - obj->height * 0.5;
	x.max = x.min + obj->height;
	y.min = (app->sdl->half_height - app->camera_angle) - obj->height * 0.5;
	y.max = y.min + obj->height;
	if (x.max > app->sdl->width)
		x.max = app->sdl->width;
	if (y.max > app->sdl->height)
		y.max = app->sdl->height;
	if ((x.cur = x.min) < 0)
		x.cur = 0;
	while (x.cur < x.max)
	{
		if ((y.cur = y.min) < 0)
			y.cur = 0;
		while (app->sdl->dist_per_x[x.cur] > obj->dist && y.cur < y.max)
		{
			draw_slice(app, obj, x, y);
			y.cur++;
		}
		x.cur++;
	}
}
