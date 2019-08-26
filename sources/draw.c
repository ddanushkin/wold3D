/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:22:52 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:22:52 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		draw_back(t_sdl *sdl, int y, int x, int end)
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

void	floor_(t_app *app, int x, int y, float angle)
{
	t_fpoint	end;
	float		dist;
	float		fish_fix;
	t_color		color;

	angle = angle * M_PI_180;
	fish_fix = cos(angle - app->player->direction * M_PI_180);
	while (y < app->sdl->height)
	{
		dist = app->sdl->dist_to_pp * (32.0) /
				(y - app->sdl->half_height + app->camera_angle) / fish_fix;
		end.y = dist * sinf(angle);
		end.x = dist * cosf(angle);
		end.y += app->player->y;
		end.x += app->player->x;
		get_color(app->textures->floors[0], &color,
				(int)end.y % 64, (int)end.x % 64);
		shade_color(dist, &color, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, y, &color);
		y++;
	}
}

void	ceiling_(t_app *app, int x, int y, float angle)
{
	t_fpoint	end;
	float		dist;
	float		fish_fix;
	t_color		color;

	angle = angle * M_PI_180;
	fish_fix = cos(angle - app->player->direction * M_PI_180);
	while (y > 0)
	{
		dist = app->sdl->dist_to_pp * (32.0) /
				(app->sdl->half_height - y - app->camera_angle) / fish_fix;
		end.y = dist * sinf(angle);
		end.x = dist * cosf(angle);
		end.y += app->player->y;
		end.x += app->player->x;
		get_color(app->textures->ceilings[0], &color,
				(int)end.y % 64, (int)end.x % 64);
		shade_color(dist, &color, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, y, &color);
		y--;
	}
}

void	draw_column(t_app *app, t_ray *ray, int x, float angle)
{
	t_iiter		y;
	double		ratio;
	t_color		col;

	ratio = TEXTURE_SIZE / ray->height;
	y.min = (app->sdl->half_height - app->camera_angle) - (ray->height * 0.5);
	y.max = y.min + ray->height;
	if (y.max > app->sdl->height)
		y.max = app->sdl->height;
	y.cur = (y.min < 0) ? 0 : y.min;
	if (ray->dist > app->sdl->draw_dist)
		y.cur = draw_back(app->sdl, y.cur, x, y.max);
	while (y.cur < y.max)
	{
		get_color(ray->texture, &col, ray->offset, (y.cur - y.min) * ratio);
		if (!(col.r == 152 && col.g == 0 && col.b == 136))
		{
			shade_color(ray->dist, &col, app->sdl->draw_dist);
			set_pixel(app->sdl, x, y.cur, &col);
		}
		y.cur++;
	}
	ceiling_(app, x, y.min, angle);
	floor_(app, x, y.max, angle);
}
