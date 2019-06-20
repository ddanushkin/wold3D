/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 17:08:24 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/19 10:28:45 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	init_time(t_time *time)
{
	ft_bzero(time, sizeof(*time));
}

void	update_time(t_time *time, t_app *app)
{
	time->prev = time->current;
	time->current = SDL_GetTicks();
	time->delta = (time->current - time->prev) * 0.001;
	time->frame += time->delta;
	app->player->speed = 250 * time->delta;
}

int		check_for_quit(t_sdl *sdl, const Uint8 *key)
{
	if (SDL_PollEvent(&sdl->event) && sdl->event.type == SDL_QUIT)
		return (1);
	if (key[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}

