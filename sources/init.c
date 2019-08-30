/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:25:06 by lglover           #+#    #+#             */
/*   Updated: 2019/08/30 16:37:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static	void	create_stuff(t_app *app, t_sdl *sdl, t_textures *textures)
{
	int			access;
	Uint32		flags;
	Uint32		format;

	format = SDL_PIXELFORMAT_RGB888;
	access = SDL_TEXTUREACCESS_STATIC;
	flags = SDL_RENDERER_SOFTWARE;
	check_for_init_errors();
	sdl->window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, sdl->width, sdl->height, 0);
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, flags);
	sdl->texture = SDL_CreateTexture(sdl->renderer, format, access,
			sdl->width, sdl->height);
	sdl->ui = load_texture(app, "./resources/", "main_ui.bmp");
	sdl->logo = load_texture(app, "./resources/main/", "logo.bmp");
	load_font(app, 60);
	textures->floors[0] = load_surface(app, "./resources/floors/", "1.bmp");
	textures->doors[0] = load_surface(app, "./resources/doors/", "D1.bmp");
	textures->ceilings[0] = load_surface(app, "./resources/ceilings/", "1.bmp");
	textures->sprites[0] = load_surface(app, "./resources/interior/", "X.bmp");
	load_walls_1(app, textures);
	load_walls_2(app, textures);
}

static	void	init_sdl(t_sdl *sdl)
{
	float	fov;

	fov = M_PI / 6.0;
	sdl->width = 1280;
	sdl->half_width = (int)(sdl->width * 0.5);
	sdl->height = 720;
	sdl->half_height = (int)(sdl->height * 0.5);
	sdl->dist_to_pp = (int)((float)sdl->half_width / tanf(fov));
	sdl->draw_dist = 840;
	sdl->pixels = (Uint32 *)malloc(sizeof(Uint32) * sdl->width * sdl->height);
	sdl->dist_per_x = (float *)malloc(sizeof(float) * sdl->width);
}

void			malloc_textures(t_textures *textures)
{
	textures->doors = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * 1);
	textures->walls = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * 36);
	textures->ceilings = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * 1);
	textures->floors = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * 1);
	textures->sprites = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * 1);
}

static	void	malloc_stuff(t_app *app)
{
	app->error = 0;
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->map = (t_map *)malloc(sizeof(t_map));
	app->map->doors_count = 0;
	app->map->objects_count = 0;
	app->player = (t_player *)malloc(sizeof(t_player));
	app->sfx = (t_sfx *)malloc(sizeof(t_sfx));
	app->time = (t_time *)malloc(sizeof(t_time));
	app->textures = (t_textures *)malloc(sizeof(t_textures));
	malloc_textures(app->textures);
}

void			init(t_app *app)
{
	malloc_stuff(app);
	init_sdl(app->sdl);
	create_stuff(app, app->sdl, app->textures);
	load_music(app, &app->sfx->background, "bgm.mp3");
	load_sound(app, &app->sfx->door_open, "door_open.wav");
	load_sound(app, &app->sfx->door_move, "door_move.wav");
	app->inputs->sensitivity = 1.5f;
	app->inputs->zoom = 300;
	app->camera_angle = 0;
	app->offset = 0;
	app->max_angle = 250;
	app->players_count = 0;
	app->animations = (t_animation *)malloc(sizeof(t_animation) * 10);
	init_idle_anim(app, &app->animations[0]);
	init_shoot_anim(app, &app->animations[1]);
	init_change_anim(app, &app->animations[2]);
	init_reload_anim(app, &app->animations[3]);
}
