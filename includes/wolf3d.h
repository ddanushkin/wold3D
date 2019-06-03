#ifndef WOLF3D_WOLF3D_H
#define WOLF3D_WOLF3D_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
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
	float			frame;
	int				prev;
	int				current;
	float			delta;
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
	SDL_Texture		*ui;
}					t_sdl;

typedef struct		s_weapon
{
	SDL_Texture		*sprites[11];
	unsigned char	ammo_cur;
	unsigned char	ammo_max;
	float			firerate;
	unsigned char	fired;
	unsigned char	mag_cur;
	Mix_Chunk 		*gun_sound;
}					t_weapon;

typedef struct		s_player
{
	int 			x;
	int 			y;
	int				health;
	int 			score;
	char			*cur_level;
	int 			lives;
	float			x_v;
	float			y_v;
	float			speed;
	int				direction;
	Mix_Music 		*bgm;
	Mix_Chunk 		*sound_effect;
	Mix_Chunk 		*fx_reload;
	Mix_Chunk 		*fx_empty;
	float			dist_w;
	float			dist_e;
	float			dist_n;
	float			dist_s;
	SDL_Texture		*state[8];
	unsigned char	shooting;
	unsigned char	changing;
	unsigned char	reloading;
	float 			anim_frame;
	t_weapon		*weapon;
	unsigned char	cur_weapon;
	unsigned char	anim_is_done;
	unsigned char	change_down;
}					t_player;

typedef struct		s_ui_elem
{
	SDL_Color		*color;
	int 			x;
	int 			y;
	int				size;
	char			*text;
}					t_ui_elem;

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
void				player_init(t_sdl *sdl, t_player *player);
void				keyboard_input(t_app *app, const Uint8 *key, float frame);
void				create_field_of_view(t_app *app);
void				shade_color(double dist, SDL_Color *color, double draw_dist);
void				get_color(SDL_Surface *surface, SDL_Color *c, int x, int y);
void				set_pixel(t_sdl *sdl, int x, int y, SDL_Color *color);
void				draw_column(t_sdl *sdl, t_ray *ray, int x, int height);
void				init_time(t_time *time);
void				update_time(t_time *time, t_app *app);
void				draw_text(SDL_Renderer	*renderer, t_ui_elem *ui_elem);
void 				create_hud(t_sdl *sdl, t_player *player);
SDL_Texture			*load_texture(t_sdl *sdl, char *name);
t_ray				*get_ray(t_map *map, t_player *player, double angle);
void				draw_face(t_sdl *sdl, t_player *player, float delta);
void 				idle_gun_animation(t_sdl *sdl, t_player *player, float delta);
void 				gun_shoot(t_sdl *sdl, t_player *player, float delta);
void				gun_change(t_sdl *sdl, t_player *player, float delta);
void 				gun_reload(t_sdl *sdl, t_player *player, float frame);
void				get_weapon_sprites(t_sdl *sdl, t_weapon *weapon, char *weapon_folder);
void				init_weapon(t_weapon *weapon, u_int ammo, float rate, char *sound);
void				update_sound(const Uint8 *key, t_player *player);
void				player_movement(t_map *map, const Uint8 *key, t_player *player);
void				player_rotate(t_player *player, const Uint8 *state);
void				redraw(t_sdl *sdl, t_player *player, t_time *time);
#endif
