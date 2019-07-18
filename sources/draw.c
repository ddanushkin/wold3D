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

void	draw_floor(t_app *app, t_ipoint tile, int x, int y, float dist)
{
	t_color		col;

	if (y > 0 && y < app->sdl->height - app->sdl->height / 5)
	{
		get_color(app->textures->floors[0], &col, tile.x, tile.y);
		shade_color(dist, &col, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, y, &col);
	}
}

void	draw_ceiling(t_app *app, t_ipoint tile, int x, int y, float dist)
{
	t_color		col;

	if (y > 0 && y < app->sdl->height - app->sdl->height / 5)
	{
		get_color(app->textures->ceilings[0], &col, tile.x, tile.y);
		shade_color(dist, &col, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, y, &col);
	}
}

void	floor_(t_app *app, int x, int y, float angle)
{
	t_ipoint cell;
	t_fpoint end;
	t_ipoint tile;
	float dist;

	angle = angle * M_PI_180;
	while (y < app->sdl->height)
	{
		dist = app->sdl->dist_to_pp * (32.0) / (y - app->sdl->half_height + app->debug_angle) / cos(angle - app->player->direction * M_PI_180);
		end.y = dist * sinf(angle);
		end.x = dist * cosf(angle);
		end.y += app->player->y;
		end.x += app->player->x;
		cell.x = end.x / 64;
		cell.y = end.y / 64;
		if (cell.x >= 0 && cell.y >= 0 &&
			cell.x < app->map->cols && cell.y < app->map->rows)
		{
			tile.y = (int)end.y % 64;
			tile.x = (int)end.x % 64;
			draw_floor(app, tile, x, y, dist);
		}
		y++;
	}
}

void	ceiling_(t_app *app, int x, int y, float angle)
{
	t_ipoint cell;
	t_fpoint end;
	t_ipoint tile;
	float dist;

	angle = angle * M_PI_180;
	while (y > 0)
	{
		dist = app->sdl->dist_to_pp * (32.0) / (app->sdl->half_height - y - app->debug_angle) / cos(angle - app->player->direction * M_PI_180);
		end.y = dist * sinf(angle);
		end.x = dist * cosf(angle);
		end.y += app->player->y;
		end.x += app->player->x;
		cell.x = end.x / 64;
		cell.y = end.y / 64;
		if (cell.x >= 0 && cell.y >= 0 &&
			cell.x < app->map->cols && cell.y < app->map->rows)
		{
			tile.y = (int)end.y % 64;
			tile.x = (int)end.x % 64;
			draw_ceiling(app, tile, x, y, dist);
		}
		y--;
	}
}

void	draw_column(t_app *app, t_ray *ray, int x, float angle)
{
	t_iiter		y;
	double		ratio;
	t_color		col;

	ratio = 64.0 / ray->height;
	y.min = (app->sdl->half_height - app->debug_angle) - (ray->height * 0.5);
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
//	floor_and_ceiling(app, x, y.max, angle);
	ceiling_(app, x, y.min, angle);
	floor_(app, x, y.max, angle);
}
