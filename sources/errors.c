/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 15:56:41 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/23 09:52:36 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_error(char *str)
{
	ft_putendl(str);
}

void	quit_properly(t_app *app)
{
	SDL_DestroyWindow(app->sdl->window);
	SDL_DestroyRenderer(app->sdl->renderer);
	SDL_DestroyTexture(app->sdl->texture);


	Mix_FreeChunk(app->player->sound_effect);
	Mix_FreeMusic(app->player->bgm);
	app->player->bgm = NULL;
	app->player->sound_effect = NULL;


	app->sdl->window = NULL;
	app->sdl->renderer = NULL;
	app->sdl->texture = NULL;
	app->sdl->pixels = NULL;
	free(app->sdl);
	free(app->sdl->pixels);
	free(app->map);
	free(app->player);

	SDL_Quit();
	Mix_Quit();
	app = NULL;
	//exit(0);
}
