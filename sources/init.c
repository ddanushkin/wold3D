/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:56:59 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/24 11:50:35 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	check_for_init_errors()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL video initialization error");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		ft_error("SDL audio initialization error");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		ft_error("Open Audio error");
	if (TTF_Init() < 0)
		ft_error("TTF error");
}

static	void	create_stuff(t_sdl *sdl)
{
	check_for_init_errors();
	sdl->window = SDL_CreateWindow("SDL2", 0, 0, sdl->width, sdl->height, 0);
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//app->sdl->screen = SDL_GetWindowSurface(app->sdl->window);
	sdl->texture = SDL_CreateTexture(sdl->renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STATIC,
			sdl->width, sdl->height);
}

static	void	init_sdl(t_sdl *sdl)
{
	check_for_init_errors();
	sdl->width = 800;
	sdl->height = 600;
	sdl->fov = 3.14159 / 3.0;
	sdl->dist_to_pp = sdl->width / (tan(sdl->fov / 2.0) * 2.0);
	sdl->draw_dist = 840;
	sdl->pixels = (Uint32 *)malloc(sizeof(Uint32) * sdl->width * sdl->height);
	ft_memset(sdl->pixels, 255, sdl->width * sdl->height * sizeof(Uint32));
	sdl->font = TTF_OpenFont("../resources/direwolf3d.ttf", 36);
	sdl->text_color.r = 144;
	sdl->text_color.g = 77;
	sdl->text_color.b = 255;
	sdl->text_color.a = 255;
}

void			init(t_app *app)
{
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->map = (t_map *)malloc(sizeof(t_map));
	app->player = (t_player *)malloc(sizeof(t_player));
	init_sdl(app->sdl);
	create_stuff(app->sdl);
}
