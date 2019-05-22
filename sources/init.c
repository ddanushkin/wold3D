/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:56:59 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/22 18:18:35 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static	void	create_stuff(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL video initialization error");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		ft_error("SDL audio initialization error");
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
	sdl->width = 640;
	sdl->height = 480;
	sdl->fov = 3.14159 / 3.0;
	sdl->dist_to_pp = sdl->width / (tan(sdl->fov / 2.0) * 2.0);
	sdl->draw_dist = 840;
}

void			init(t_app *app)
{
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->map = (t_map *)malloc(sizeof(t_map));
	app->player = (t_player *)malloc(sizeof(t_player));
	init_sdl(app->sdl);
	create_stuff(app->sdl);
	app->sdl->pixels = (Uint32 *)malloc(sizeof(Uint32) * app->sdl->width * app->sdl->height);
	ft_memset(app->sdl->pixels, 255, app->sdl->width * app->sdl->height * sizeof(Uint32));
}
