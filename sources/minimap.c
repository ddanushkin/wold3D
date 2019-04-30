#include "wolf3d.h"

void	draw_sector(t_sdl *sdl, int row, int col)
{
	int i;
	int j;

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

void	draw_player(t_sdl *sdl, t_player *player)
{
	t_ipoint s;
	t_ipoint e;
	static t_ipoint cur;
	int flag = 0;
	int i;

	i = 0;
	s.x = player->x;
	s.y = player->y;

	if (!cur.x && !cur.y)
	{
		cur = s;
		flag = 1;
	}
	if (cur.x == s.x && cur.y == s.y && flag != 1)
	{
		return;
	}
	flag = 0;
	SDL_SetRenderDrawColor(sdl->ren, 0, 0, 0, 255);
/*	end_point(player->direction, s, &e, 100);
	while(i < 45)
	{
		e.y += 1;
		line_add(sdl, s, e);
		i++;
	}*/
	player->direction -= 60;
	while(i < 61)
	{
		end_point(player->direction, s, &e, 100);
		line_add(sdl, s, e);
		player->direction++;
		i++;
	}
	SDL_SetRenderDrawColor(sdl->ren, 255, 0, 0, 255);
	SDL_RenderDrawPoint(sdl->ren, e.x, e.y);
	SDL_SetRenderDrawColor(sdl->ren, 2, 191, 255, 255);
	SDL_RenderDrawPoint(sdl->ren, player->x, player->y);
	//SDL_SetRenderDrawColor(sdl->ren, 0, 0, 0, 0);
}

void	minimap_draw(t_map *map, t_sdl *sdl, t_player *player)
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
			{
				draw_sector(sdl, row, col);
				//draw_player(sdl, player);
			}
			col++;
		}
		row++;
	}
	draw_player(sdl, player);
}
