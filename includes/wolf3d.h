#ifndef WOLF3D_WOLF3D_H
#define WOLF3D_WOLF3D_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> /* Delete! */
#include "libft.h"

# define TEXTURE_SIZE 64
# define M_PI_180 0.017453292519943295
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

typedef	struct		s_time
{
	float		frame;
	int			prev;
	int			current;
	float		delta;
}					t_time;

typedef struct		s_sdl
{
	int 			width;
	int 			height;
	int				dist_to_pp;
	float			fov;
	int 			draw_dist;
	SDL_Event		event;
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	Uint32			*pixels;
	SDL_Surface		*screen;
}					t_sdl;

typedef struct		s_player
{
	int 			x;
	int 			y;
	float			x_v;
	float			y_v;
	float			speed;
	int				direction;
	Mix_Music 		*bgm;
	Mix_Chunk 		*sound_effect;
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
void				keyboard_input(t_map *map, const Uint8 *key, t_player *player);
void				create_field_of_view(t_app *app);
void				shade_color(double dist, SDL_Color *color, double shade_dist);
void				get_color(SDL_Surface *surface, SDL_Color *c, int x, int y);
void				set_pixel(t_sdl *sdl, int x, int y, SDL_Color *color);
void				draw_column(t_sdl *sdl, t_ray *ray, int x, int height);
void				init_time(t_time *time);
void				update_time(t_time *time, t_app *app);
#endif
