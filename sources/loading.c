/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:25:42 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 18:24:52 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

SDL_Surface		*load_surface(t_app *app, char *folder, char *name)
{
	char		file_path[50];
	SDL_Surface	*surf;

	ft_strcpy(file_path, folder);
	ft_strcat(file_path, name);
	surf = SDL_LoadBMP(file_path);
	if (surf != NULL && surf->w == 64 && surf->h == 64)
		return (surf);
	else
	{
		app->error = 1;
		return (NULL);
	}
}

SDL_Texture		*load_texture(t_app *app, char *folder,
								char *name)
{
	char			file_path[50];
	Uint32			key;
	SDL_Surface		*surface;
	SDL_Texture		*texture;

	ft_strcpy(file_path, folder);
	ft_strcat(file_path, name);
	surface = SDL_LoadBMP(file_path);
	if (surface != NULL)
	{
		key = SDL_MapRGB(surface->format, 152, 0, 136);
		SDL_SetColorKey(surface, SDL_TRUE, key);
		texture = SDL_CreateTextureFromSurface(app->sdl->renderer, surface);
		SDL_FreeSurface(surface);
		return (texture);
	}
	else
	{
		app->error = 1;
		return (NULL);
	}
}

void			load_walls(t_app *app, t_textures *textures)
{
	textures->walls[0] = load_surface(app, "./resources/walls/", "1_e.bmp");
	textures->walls[1] = load_surface(app, "./resources/walls/", "1_n.bmp");
	textures->walls[2] = load_surface(app, "./resources/walls/", "1_s.bmp");
	textures->walls[3] = load_surface(app, "./resources/walls/", "1_w.bmp");
	textures->walls[4] = load_surface(app, "./resources/walls/", "2_e.bmp");
	textures->walls[5] = load_surface(app, "./resources/walls/", "2_n.bmp");
	textures->walls[6] = load_surface(app, "./resources/walls/", "2_s.bmp");
	textures->walls[7] = load_surface(app, "./resources/walls/", "2_w.bmp");
	textures->walls[8] = load_surface(app, "./resources/walls/", "3_e.bmp");
	textures->walls[9] = load_surface(app, "./resources/walls/", "3_n.bmp");
	textures->walls[10] = load_surface(app, "./resources/walls/", "3_s.bmp");
	textures->walls[11] = load_surface(app, "./resources/walls/", "3_w.bmp");
}

void			load_sound(t_app *app, Mix_Chunk *sound, char *name)
{
	char		file_path[50];

	file_path[0] = '\0';
	ft_strcpy(file_path, "./resources/sounds/");
	ft_strcat(file_path, name);
	sound = Mix_LoadWAV(file_path);
	if (sound == NULL)
		app->error = 1;
}

void			load_music(t_app *app, Mix_Music *music, char *name)
{
	char		file_path[50];

	file_path[0] = '\0';
	ft_strcpy(file_path, "./resources/sounds/");
	ft_strcat(file_path, name);
	music = Mix_LoadMUS(file_path);
	if (music == NULL)
		app->error = 1;
}
