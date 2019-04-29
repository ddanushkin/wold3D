/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 12:28:27 by lglover           #+#    #+#             */
/*   Updated: 2019/04/29 12:38:34 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char			*texture;
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

#endif
