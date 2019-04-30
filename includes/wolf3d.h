#ifndef WOLF3D_WOLF3D_H
#define WOLF3D_WOLF3D_H

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> /* Delete! */
#include "libft.h"

typedef struct		s_sdl
{
	SDL_Window		*win;
	int 			width;
	int 			height;
	SDL_Renderer	*ren;
	SDL_Event		event;
}					t_sdl;

typedef struct		s_player
{
	int				x;
	int				y;
	int				d;
	int				v;
	int				a;
}					t_player;

typedef struct		s_node
{
	int				x;
	int				y;
	char 			*name;
	SDL_Surface		*texture;
	int 			collidable;
}					t_node;

typedef struct		s_map
{
	int				rows;
	int 			cols;
	t_node			**nodes;
}					t_map;

typedef struct		s_app
{
	t_sdl			sdl;
	t_player		player;
	t_map			map;
}					t_app;

# define MM_SEC_SIZE 16

void				init(t_sdl *sdl, t_player *player);
void				read_map(int fd, t_map *map, t_player *player);
void				ft_error(char *str);
void				quit_properly(t_app *sdl);
void				draw_minimap(t_map *map, t_sdl *sdl, t_player *player);
int					loadMedia(t_node *node);
#endif
