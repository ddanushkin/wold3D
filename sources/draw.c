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

void	draw_floor(t_app *app, t_ipoint tile, int x, int y)
{
	t_color		col;

	if (y > 0 && y < app->sdl->height - app->sdl->height / 5)
	{
		get_color(app->textures->floors[0], &col, tile.x, tile.y);
		shade_color(app->sdl->diag_dist[y - (int)app->player->head_offset],
					&col, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, y, &col);
	}
}

void	draw_ceiling(t_app *app, t_ipoint tile, int x, int y)
{
	t_color		col;

	if (app->sdl->height - y > 0 &&
		app->sdl->height - y < app->sdl->height)
	{
		get_color(app->textures->ceilings[0], &col, tile.x, tile.y);
		shade_color(app->sdl->diag_dist[y + (int)app->player->head_offset],
					&col, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, app->sdl->height - y, &col);
	}
}

void	floor_and_ceiling(t_app *app, int x, int y, float angle)
{
	t_ipoint cell;
	t_fpoint end;
	t_ipoint tile;

	angle = angle * M_PI_180;
	while (y < app->sdl->height)
	{
		end.y = app->sdl->diag_dist[y] * sinf(angle);
		end.x = app->sdl->diag_dist[y] * cosf(angle);
		end.y += app->player->y;
		end.x += app->player->x;
		cell.x = end.x / 64;
		cell.y = end.y / 64;
		if (cell.x >= 0 && cell.y >= 0 &&
			cell.x < app->map->cols && cell.y < app->map->rows)
		{
			tile.y = (int)end.y % 64;
			tile.x = (int)end.x % 64;
			draw_floor(app, tile, x, y + app->player->head_offset);
			draw_ceiling(app, tile, x, y - app->player->head_offset);
		}
		y++;
	}
}

void	draw_column(t_app *app, t_ray *ray, int x, float angle)
{
	t_iiter		y;
	double		ratio;
	t_color		col;

	ratio = 64.0 / ray->height;
	y.min = (app->sdl->height - ray->height) * 0.5 + app->player->head_offset;
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
	floor_and_ceiling(app, x, y.max - app->player->head_offset, angle);
}
