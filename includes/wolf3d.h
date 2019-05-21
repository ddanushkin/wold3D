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
	int 			x;
	int 			y;
}					t_ray;

typedef struct		s_sdl
{
	int 			width;
	int 			height;
	int				dist_to_pp;
	float			fov;
	int 			draw_dist;
	double			ceiling;
	Uint32 			delta_time;
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
	SDL_Surface		*texture_n;
	SDL_Surface		*texture_s;
	SDL_Surface		*texture_e;
	SDL_Surface		*texture_w;
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
	t_sdl			*sdl;
	t_player		*player;
	t_map			*map;
}					t_app;

void				init(t_app *app);
void				map_read(int fd, t_map *map, t_player *player);
void				ft_error(char *str);
void				quit_properly(t_app *sdl);
void				draw_minimap(t_map *map, t_sdl *sdl, t_player *player);
void				player_init(t_player *player);
void				player_move(t_map *map, const Uint8	*state, t_player *player);
void				cast_rays(t_app *app);
void				shade_color(double dist, SDL_Color *color, double shade_dist);
void				get_color(SDL_Surface *surface, SDL_Color *c, int x, int y);
void				set_pixel(t_sdl *sdl, int x, int y, SDL_Color *color);
void				draw_col(t_sdl * sdl, t_ray *ray, int x, int height);
#endif
