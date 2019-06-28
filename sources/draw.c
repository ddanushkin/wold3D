#include "wolf3d.h"

SDL_Texture	*load_texture(t_sdl *sdl, char *name)
{
	char		file_path[50];
	SDL_Surface	*surface;
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

	if (y - app->player->head_offset > 0 &&
		y - app->player->head_offset < app->sdl->height - app->sdl->height / 5)
	{
		get_color(app->textures->floors, &c, tile.x, tile.y);
		shade_color(app->diag_dist[y], &c, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, y - app->player->head_offset, &c);
	}
}

void	draw_ceiling(t_app *app, t_ipoint tile, int x, int y)
{
	SDL_Color c;

	if (app->sdl->height - y - app->player->head_offset > 0 &&
		app->sdl->height - y - app->player->head_offset < app->sdl->height)
	{
		get_color(app->textures->ceilings, &c, tile.x, tile.y);
		shade_color(app->diag_dist[y], &c, app->sdl->draw_dist - 100);
		set_pixel(app->sdl, x, app->sdl->height - y - app->player->head_offset, &c);
	}
}

void	floor_and_ceiling(t_app *app, int x, int y, float angle)
{
	t_fpoint end;
	t_ipoint cell;
	t_ipoint tile;

	angle = angle * M_PI_180;
	y =  y + app->player->head_offset;
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
			draw_floor(app, tile, x, y);
			draw_ceiling(app, tile, x, y);
		}
		y++;
	}
}

void		draw_column(t_app *app, t_ray *ray, int x, int height, float angle)
{
	int			y;
	int			begin;
	int			end;
	double		ratio;
	SDL_Color	color;

	ratio = 64.0 / height;
	begin = (app->sdl->height / 2) - app->player->head_offset - (height / 2);
	if ((end = begin + height) > app->sdl->height)
		end = app->sdl->height;
	y = (begin < 0) ? 0 : begin;
	if (ray->dist > app->sdl->draw_dist)
		y = draw_back(app->sdl, y, x, end);
	while (y < end)
	{
		get_color(ray->texture, &color, ray->offset, (y - begin) * ratio);
		if (!(color.r == 152 && color.g == 0 && color.b == 136))
		{
			shade_color(ray->dist, &color, app->sdl->draw_dist);
			set_pixel(app->sdl, x, y, &color);
		}
		y++;
	}
	floor_and_ceiling(app, x, end, angle);
	app->dist_per_x[x] = ray->dist;
}
