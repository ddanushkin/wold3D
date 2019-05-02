#ifndef WOLF3D_WOLF3D_H
#define WOLF3D_WOLF3D_H

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> /* Delete! */
#include "libft.h"

# define MM_SEC_SIZE 32
# define TICK_INTERVAL 30
# define MAX(a,b) (((a)>(b))?(a):(b))

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
	int 			y;
	int				x_v;
	int				y_v;
	int				speed;
	int 			direction;
	double			x_vector;
	double			y_vector;
	double 			x_plane;
	double 			y_plane;
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

void				init(t_sdl *sdl);
void				map_read(int fd, t_map *map, t_player *player);
void				ft_error(char *str);
void				quit_properly(t_app *sdl);
void				minimap_draw(t_map *map, t_sdl *sdl, t_player *player);
int					load_texture(t_node *node, char *name);
void				player_init(t_player *player);
void				player_vel(t_player *player, const Uint8 *state);
void				player_move(t_player *player);
double				to_rad(int angle);
void				end_point(int angle, t_ipoint s, t_ipoint *e, int length);
void				cast_ray(t_sdl *sdl, t_map *map, t_player *player, int a);
#endif
