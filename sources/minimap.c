#include "wolf3d.h"

void		draw_sector(t_sdl *sdl, int row, int col)
{
	int		i;
	int		j;

	i = 0;
	SDL_SetRenderDrawColor(sdl->ren, 163, 163, 163, 255);
	while (i < MM_SEC_SIZE)
	{
		j = 0;
		while (j < MM_SEC_SIZE)
			SDL_RenderDrawPoint(sdl->ren, col*MM_SEC_SIZE+i, row*MM_SEC_SIZE+(j++));
		i++;
	}
	SDL_SetRenderDrawColor(sdl->ren, 0, 0, 0, 0);
}

void		draw_player(t_sdl *sdl, t_map *map, t_player *player)
{
	cast_rays(sdl, map, player, 60);
	player_is_collising(map, player);
	SDL_SetRenderDrawColor(sdl->ren, 2, 191, 255, 255);
	SDL_RenderDrawPoint(sdl->ren, player->x, player->y);
}

void		minimap_draw(t_map *map, t_sdl *sdl, t_player *player)
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
