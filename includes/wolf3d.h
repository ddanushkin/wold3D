#ifndef WOLF3D_WOLF3D_H
#define WOLF3D_WOLF3D_H

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> /* Delete! */
#include "libft.h"

# define TEXTURE_SIZE 64
# define TICK_INTERVAL 30
# define MAX(a,b) (a > b) ? a : b

typedef struct		s_ipoint
{
	int				x;
	int				y;
}					t_ipoint;

typedef struct		s_fpoint
{
	float			x;
	float			y;
}					t_fpoint;

typedef struct		s_dpoint
{
	double			x;
	double			y;
}					t_dpoint;

typedef struct		s_ray
{
	int				offset;
	float			dist;
	SDL_Surface		*texture;
}					t_ray;

typedef struct		s_sdl
{
	int 			width;
	int 			height;
	int				dist_to_pp;
	float			fov;
	SDL_Event		event;
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Surface		*screen;
}					t_sdl;

typedef struct		s_player
{
	int 			x;
	int 			y;
	double			x_v;
	double			y_v;
	int				speed;
	int				direction;
	double			shade_dist;
	int 			max_dist;
}					t_player;

typedef struct		s_node
{
	int				x;
	int				y;
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

typedef struct		s_texture_col
{
	SDL_Surface		*surface;
	SDL_Color		**pixels;
	int				height;
}					t_texture_col;

void				init(t_sdl *sdl);
void				map_read(int fd, t_map *map, t_player *player);
void				ft_error(char *str);
void				quit_properly(t_app *sdl);
void				draw_minimap(t_map *map, t_sdl *sdl, t_player *player);
int					load_texture(t_node *node, char *name);
void				player_init(t_player *player);
void				player_move(t_map *map, const Uint8	*state, t_player *player);
void				cast_rays(t_sdl *sdl, t_map *map, t_player *player);
void				get_rgb(SDL_Surface *surface, SDL_Color *c, int x, int y);
#endif
