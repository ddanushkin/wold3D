/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:53 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:26:53 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	init_time(t_time *time)
{
	ft_bzero(time, sizeof(*time));
	time->current = SDL_GetTicks();
}

void	update_time(t_time *time)
{
	time->prev = time->current;
	time->current = SDL_GetTicks();
	time->delta = (time->current - time->prev) * 0.001;
	time->frame += time->delta;
	time->frame_nbr++;
	if (time->frame_nbr < 0)
		time->frame_nbr = 0;
}

int		check_for_quit(SDL_Event *event, t_inputs *inputs)
{
	if (event->type == SDL_QUIT)
		return (1);
	if (inputs->keyboard[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}

void	shoot_weapon(t_app *app)
{
	t_weapon		*weapon;

	weapon = &app->player->weapon[app->player->cur_weapon];
	if (weapon->ammo_cur != 0)
	{
		weapon->ammo_cur--;
		app->player->last_shoot = 1;
		Mix_PlayChannel(1, weapon->gun_sound, 0);
	}
	else
		Mix_PlayChannel(1, app->player->fx_empty, 0);
	weapon->fired = 1;
}
