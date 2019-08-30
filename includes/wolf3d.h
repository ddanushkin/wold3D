/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:27:25 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 16:16:47 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_WOLF3D_H
# define WOLF3D_WOLF3D_H

# include <SDL.h>
# include <SDL_mixer.h>
# include <SDL_ttf.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <dirent.h>
# include "libft.h"

# define TEXTURE_SIZE 64
# define M_PI_180 0.017453292519943295

# define MAP_TYPE_EMPTY 0
# define MAP_TYPE_WALL 1
# define MAP_TYPE_INTERIOR 2
# define MAP_TYPE_DOOR 3

# define RAY_TYPE_HORZ 1
# define RAY_TYPE_VERT 0

# define PL_STATE_IDLE 0
# define PL_STATE_SHOOT 1
# define PL_STATE_RELOAD 2
# define PL_STATE_CHANGE 3

# define ANIM_IDLE 0
# define ANIM_SHOOT 1
# define ANIM_CHANGE 2
# define ANIM_RELOAD 3
# define ANIM_COUNT 4

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

typedef struct		s_iiter
{
	int				min;
	int				cur;
	int				max;
}					t_iiter;

typedef struct		s_animation
{
	SDL_Rect		*area;
	int				play;
	int				current_frame;
	int				total_frames;
	int				loop;
	float			delay;
	float			counter;
	float			speed;
	float			delayed;
}					t_animation;

typedef	struct		s_time
{
	int				current;
	float			delta;
	float			frame;
	int				prev;
	int				frame_nbr;
}					t_time;

typedef struct		s_sdl
{
	float			*diag_dist;
	float			*dist_per_x;
	int				dist_to_pp;
	int				draw_dist;
	SDL_Event		event;
	int				half_height;
	int				half_width;
	int				height;
	Uint32			*pixels;
	SDL_Renderer	*renderer;
	SDL_Renderer	*renderer2;
	SDL_Texture		*texture;
	SDL_Texture		*ui;
	int				width;
	SDL_Window		*window;
	TTF_Font		*font;
	SDL_Texture		*logo;
	SDL_Texture		*main;
}					t_sdl;

typedef struct		s_weapon
{
	unsigned char	ammo_cur;
	unsigned char	ammo_max;
	unsigned char	fired;
	float			firerate;
	Mix_Chunk		*gun_sound;
	unsigned char	mag_cur;
	SDL_Texture		*sprites[11];
}					t_weapon;

typedef struct		s_node
{
	t_ipoint		center;
	u_int			collidable;
	float			dist;
	int				door_closing;
	int				door_frame;
	int				door_opening;
	float			height;
	float			last_open;
	int				screen_x;
	SDL_Surface		*texture[4];
	u_int			type;
	int				visible;
	int				x;
	int				y;
}					t_node;

typedef struct		s_ray
{
	float			dist;
	t_node			*node;
	int				offset;
	t_fpoint		start;
	t_fpoint		step;
	SDL_Surface		*texture;
	int				type;
	float			height;
}					t_ray;

typedef struct		s_player
{
	char			*cur_level;
	unsigned char	cur_weapon;
	float			direction;
	Mix_Chunk		*fx_die;
	Mix_Chunk		*fx_empty;
	Mix_Chunk		*fx_reload;
	float			head_acc;
	float			head_offset;
	int				health;
	float			idle_frame;
	float			last_shift;
	float			last_space;
	int				step;
	int				changed;
	int				reloaded;
	int				last_shoot;
	int				lives;
	int				max_dist;
	float			move_acc;
	int				score;
	Mix_Chunk		*sound_effect;
	float			speed;
	SDL_Texture		*faces[28];
	Mix_Chunk		*steps[8];
	t_weapon		*weapon;
	int				state;
	int				x;
	float			x_v;
	int				y;
	float			y_v;

}					t_player;

typedef struct		s_ui_elem
{
	SDL_Color		*color;
	int				size;
	char			*text;
	int				x;
	int				y;
}					t_ui_elem;

typedef struct		s_map
{
	int				cols;
	int				rows;
	t_node			**nodes;
	t_node			**doors;
	int				doors_count;
	t_node			**objects;
	int				objects_count;
}					t_map;

typedef struct		s_sfx
{
	Mix_Music		*background;
	Mix_Chunk		*door_move;
	Mix_Chunk		*door_open;
}					t_sfx;

typedef struct		s_textures
{
	SDL_Surface		**ceilings;
	SDL_Surface		**doors;
	SDL_Surface		**floors;
	SDL_Surface		**sprites;
	SDL_Surface		**walls;
}					t_textures;

typedef struct		s_inputs
{
	const Uint8		*keyboard;
	int				left_pressed;
	int				right_pressed;
	float			sensitivity;
	int				x;
	int				y;
	int				zoom;
}					t_inputs;

typedef struct		s_app
{
	t_inputs		*inputs;
	t_map			*map;
	t_player		*player;
	t_sdl			*sdl;
	t_sfx			*sfx;
	t_textures		*textures;
	t_time			*time;
	t_animation		*animations;
	int				offset;
	float			camera_angle;
	float			max_angle;
	int				players_count;
	int				error;
}					t_app;

typedef struct		s_color
{
	Uint8			r;
	Uint8			g;
	Uint8			b;
}					t_color;

void				init(t_app *app);
int					map_read(int fd, t_app *app);
void				ft_error(char *str);
void				quit_properly(t_app *sdl);
void				player_init(t_app *app, t_player *player);
void				keyboard_input(t_app *app, float frame);
void				create_field_of_view(t_app *app);
void				get_color(SDL_Surface *surface, t_color *c, int x, int y);
void				shade_color(double dist, t_color *color, double draw_dist);
void				set_pixel(t_sdl *sdl, int x, int y, t_color *col);
void				draw_obj_column(t_sdl *sdl, t_ray *ray, int x, int height);
void				init_time(t_time *time);
void				update_time(t_time *time);
void				draw_text_font(SDL_Renderer *renderer, t_ui_elem *ui_elem,
						TTF_Font *font);
void				create_hud(t_sdl *sdl, t_player *player);
t_ray				*get_ray(t_app *app, int x, float angle);
void				draw_face(t_sdl *sdl, t_player *player, float delta);
int					animation_ended(t_app *app, t_animation *anim);
void				init_weapon(t_weapon *weapon, u_int ammo, float rate);
void				update_sound(const Uint8 *key, t_sfx *sfx);
void				player_movement(t_node **nodes, const Uint8 *key,
									t_player *player);
void				redraw(t_app *app, float time);
int					check_for_quit(SDL_Event *event, t_inputs *inputs);
void				update_doors(t_app *app, float frame);
void				door_interaction(t_app *app, float frame);
void				draw_column(t_app *app, t_ray *ray, int x, float angle);
void				map_type_wall(t_app *app, t_node *node, int index);
void				map_type_interior(t_app *app, t_node *node);
void				map_type_door(t_app *app, t_node *node);
void				update_objects(t_app *app);
void				draw_object(t_app *app, t_node *obj);
void				on_mouse_update(t_app *app);
SDL_Surface			*load_surface(t_app *app, char *folder, char *sprite);
SDL_Texture			*load_texture(t_app *app, char *folder, char *sprite);
void				init_idle_anim(t_app *app, t_animation *anim);
void				init_shoot_anim(t_app *app, t_animation *anim);
void				init_change_anim(t_app *app, t_animation *anim);
void				init_reload_anim(t_app *app, t_animation *anim);
void				idle_draw(t_app *app, t_animation *anim);
void				shoot_draw(t_app *app, t_animation *anim);
void				change_draw(t_app *app, t_animation *anim);
void				reload_draw(t_app *app, t_animation *anim);
void				animation_start(t_animation *anim);
void				animations_update(t_app *app);
void				animation_next_frame(t_animation *anim);
void				map_init(t_app *app);
void				fill_row(t_app *app, char **data, int row);
void				node_reset(t_node *node, int row, int col);
void				map_type_empty(t_node *node);
int					map_count_cols(char *row, t_map *map);
int					map_count_rows(char *str, t_map *map);
void				shoot_weapon(t_app *app);
void				change_weapon(t_app *app);
void				reload_weapon(t_app *app);
void				state_change(t_app *app);
void				place_player(t_app *app, t_node *node,
								int row, int col);
void				update_level(SDL_Renderer *renderer, char *level,
						t_ui_elem *ui_elem, TTF_Font *font);
void				update_score(SDL_Renderer *renderer, int score,
						t_ui_elem *ui_elem, TTF_Font *font);
void				update_life(SDL_Renderer *renderer, int lives,
						t_ui_elem *ui_elem, TTF_Font *font);
void				update_health(SDL_Renderer *renderer, int health,
						t_ui_elem *ui_elem, TTF_Font *font);
void				update_ammo(SDL_Renderer *renderer, t_player *player,
						t_ui_elem *ui_elem, TTF_Font *font);
void				animation_idle_weapon(t_app *app);
void				animation_change_weapon(t_app *app);
void				animation_reload_weapon(t_app *app);
void				animation_shoot_weapon(t_app *app);
int					is_wall(t_map *map, t_ray *ray);
void				calc_wall_data(t_ray *ray, float angle);
void				calc_door_data(t_ray *ray);
t_ray				*init_horz(int x, int y, float angle);
t_ray				*init_vert(int x, int y, float angle);
void				load_sounds(t_app *app, t_player *player);
void				load_weapons(t_app *app, t_player *player);
void				load_faces(t_app *app, t_player *player);
t_node				*get_node(t_node **nodes, float x, float y);
void				draw_view(t_app *app);
void				fix_direction(t_app *app);
void				check_for_init_errors(void);
void				init_weapon_sprite(t_app *app, t_weapon *weapon,
										char *weapon_name);
void				load_walls_1(t_app *app, t_textures *textures);
void				load_walls_2(t_app *app, t_textures *textures);
void				load_sound(t_app *app, Mix_Chunk **sound, char *name);
void				load_music(t_app *app, Mix_Music **music, char *name);
void				load_font(t_app *app, int size);
#endif
