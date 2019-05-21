#include "wolf3d.h"

void		draw_player(t_sdl *sdl, t_map *map, t_player *player)
{
	SDL_SetRenderDrawColor(sdl->renderer, 195, 0, 255, 255);
	SDL_RenderDrawPoint(sdl->renderer, player->x, player->y - 1);
	SDL_RenderDrawPoint(sdl->renderer, player->x - 1, player->y);
	SDL_RenderDrawPoint(sdl->renderer, player->x, player->y + 1);
	SDL_RenderDrawPoint(sdl->renderer, player->x + 1, player->y);
	SDL_RenderDrawPoint(sdl->renderer, player->x + 1, player->y - 1);
	SDL_RenderDrawPoint(sdl->renderer, player->x - 1, player->y + 1);
	SDL_RenderDrawPoint(sdl->renderer, player->x + 1, player->y + 1);
	SDL_RenderDrawPoint(sdl->renderer, player->x - 1, player->y - 1);
	SDL_RenderDrawPoint(sdl->renderer, player->x, player->y);
}

void		draw_sector(t_sdl *sdl, int row, int col)
{
	int		i;
	int		j;

	i = 0;
	col = col * TEXTURE_SIZE;
	row = row * TEXTURE_SIZE;
	SDL_SetRenderDrawColor(sdl->renderer, 163, 163, 163, 255);
	while (i < TEXTURE_SIZE)
	{
		j = 0;
		while (j < TEXTURE_SIZE)
			SDL_RenderDrawPoint(sdl->renderer, col+i, row+(j++));
		i++;
	}
}

void		draw_minimap(t_map *map, t_sdl *sdl, t_player *player)
{
	int		row;
	int		col;

	row = 0;
	while (row < map->rows)
	{
		col = 0;
		while (col < map->cols)
		{
			if (map->nodes[row][col].collidable)
				draw_sector(sdl, row, col);
			col++;
		}
		row++;
	}
	draw_player(sdl, map, player);
}
