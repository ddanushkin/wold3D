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

static void	draw_floor2(t_app *app, int x, int y, float angle, int start)
{
	int end;

	float ratio;
	float diagonalDistance;
	float yEnd;
	float xEnd;
	int cellX;
	int cellY;
	int tileRow;
	int tileColumn;
	SDL_Color c;



	angle = angle * M_PI_180;

	y =  y + app->player->head_offset;
	while (y < app->sdl->height)
	{
		ratio = app->player->height / (y - app->sdl->height / 2);
		diagonalDistance = app->sdl->dist_to_pp * ratio;

		yEnd = diagonalDistance * sinf(angle);
		xEnd = diagonalDistance * cosf(angle);

		yEnd += app->player->y;
		xEnd += app->player->x;

		cellX = xEnd / 64;
		cellY = yEnd / 64;
		if (cellX >= 0 && cellY >= 0 && cellX < app->map->cols && cellY < app->map->rows)
		{
			tileRow = (int)yEnd % 64;
			tileColumn = (int)xEnd % 64;
			if (y - app->player->head_offset > 0 && y - app->player->head_offset < app->sdl->height - app->sdl->height / 5)
			{
				get_color(app->textures->floors, &c,tileColumn, tileRow);
				shade_color(diagonalDistance, &c, app->sdl->draw_dist - 100);
				set_pixel(app->sdl, x, y - app->player->head_offset , &c);

			}
			if (app->sdl->height - y - app->player->head_offset > 0 && app->sdl->height - y - app->player->head_offset < app->sdl->height)
			{
				get_color(app->textures->ceilings, &c,tileColumn, tileRow);
				shade_color(diagonalDistance, &c, app->sdl->draw_dist - 100);
				set_pixel(app->sdl, x, app->sdl->height - y - app->player->head_offset, &c);
			}
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
	int start = y;
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
	draw_floor2(app, x, end, angle, start);
}
