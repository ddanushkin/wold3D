#include "wolf3d.h"

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

void	draw_floor(t_app *app, t_ipoint tile, int x, int y)
{
	SDL_Color c;

	if (y > 0 && y < app->sdl->height - app->sdl->height / 5)
	{
		get_color(app->textures->floors, &c, tile.x, tile.y);
		shade_color(app->diag_dist[y - (int)app->player->head_offset],
				&c, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, y, &c);
	}
}

void	draw_ceiling(t_app *app, t_ipoint tile, int x, int y)
{
	SDL_Color c;

	if (app->sdl->height - y > 0 &&
		app->sdl->height - y < app->sdl->height)
	{
		get_color(app->textures->ceilings, &c, tile.x, tile.y);
		shade_color(app->diag_dist[y + (int)app->player->head_offset],
				&c, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, app->sdl->height - y, &c);
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
		end.y = app->diag_dist[y] * sinf(angle);
		end.x = app->diag_dist[y] * cosf(angle);
		end.y += app->player->y;
		end.x += app->player->x;
		cell.x = end.x / 64;
		cell.y = end.y / 64;
		if (cell.x >= 0 && cell.y >= 0 &&
			cell.x < app->map->cols && cell.y < app->map->rows)
		{
			tile.y = (int)end.y % 64;
			tile.x = (int)end.x % 64;
			draw_floor(app, tile, x, (int)(y + app->player->head_offset));
			draw_ceiling(app, tile, x, (int)(y - app->player->head_offset));
		}
		y++;
	}
}

/*void		draw_object(t_app *app, t_node *obj)
{
	t_ipoint	start;
	t_ipoint	end;
	int			x;
	int			y;

	start.x = obj->screen_x - obj->height * 0.5;
	start.y = app->sdl->half_height - obj->height * 0.5;
	end.x = start.x + obj->height;
	end.y = start.y + obj->height;
	if (start.y < 0)
		start.y = 0;
	if (end.y > app->sdl->height)
		start.y = app->sdl->height;
	x = start.x;
	while (x < end.x)
	{
		y = start.y;
		while (app->sdl->dist_per_x[x] > obj->dist && y < end.y)
		{
			int			texture_x;
			int			texture_y;
			SDL_Color	color;
			texture_x = (end.x - x) * 64.0 / obj->height;
			texture_y = 64.0 - (end.y - y) * 64.0 / obj->height;
			get_color(obj->texture[0], &color, texture_x, texture_y);
			if (!(color.r == 152 && color.g == 0 && color.b == 136) &&
				y + app->player->head_offset >= 0 && y + app->player->head_offset < app->sdl->height)
			{
				shade_color(obj->dist + 150, &color, app->sdl->draw_dist);
				set_pixel(app->sdl, x, (int)(y + app->player->head_offset), &color);
			}
			y++;
		}
		x++;
	}
}*/

void		draw_object(t_sdl *sdl,t_player *player, t_node *obj)
{
	t_ipoint	start;
	t_ipoint	end;
	int			x;
	int			y;

	start.x = obj->screen_x - obj->height * 0.5;
	start.y = sdl->half_height - obj->height * 0.5;
	end.x = start.x + obj->height;
	end.y = start.y + obj->height;
	if (start.y < 0)
		start.y = 0;
	if (end.y > sdl->height)
		start.y = sdl->height;
	x = start.x;
	while (x < end.x)
	{
		y = start.y;
		while (sdl->dist_per_x[x] > obj->dist && y < end.y)
		{
			int			texture_x;
			int			texture_y;
			SDL_Color	color;
			texture_x = (end.x - x) * 64.0 / obj->height;
			texture_y = 64.0 - (end.y - y) * 64.0 / obj->height;
			get_color(obj->texture[0], &color, texture_x, texture_y);
			if (!(color.r == 152 && color.g == 0 && color.b == 136) &&
				y + player->head_offset >= 0 && y + player->head_offset < sdl->height)
			{
				shade_color(obj->dist + 150, &color, sdl->draw_dist);
				set_pixel(sdl, x, (int)(y + player->head_offset), &color);
			}
			y++;
		}
		x++;
	}
}

void		draw_column(t_app *app, t_ray *ray, int x, float angle)
{
	int			begin;
	SDL_Color	color;
	int			end;
	int			slice_height;
	int			y;

	slice_height = (int)(64 / ray->dist * app->sdl->dist_to_pp);
	begin = (app->sdl->height - slice_height) * 0.5 + app->player->head_offset;
	if ((end = begin + slice_height) > app->sdl->height)
		end = app->sdl->height;
	y = (begin < 0) ? 0 : begin;
	if (ray->dist > app->sdl->draw_dist)
		y = draw_back(app->sdl, y, x, end);
	while (y < end)
	{
		get_color(ray->texture, &color, ray->offset,
				(y - begin) * 64.0 / slice_height);
		if (!(color.r == 152 && color.g == 0 && color.b == 136))
		{
			shade_color(ray->dist, &color, app->sdl->draw_dist);
			set_pixel(app->sdl, x, y, &color);
		}
		y++;
	}
	floor_and_ceiling(app, x, (int)(end - app->player->head_offset), angle);
	app->dist_per_x[x] = ray->dist;
}
