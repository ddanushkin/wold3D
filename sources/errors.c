/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 15:56:56 by ndremora          #+#    #+#             */
/*   Updated: 2019/04/29 18:11:30 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_error(char *str)
{
	ft_putendl(str);
	exit(0);
}

void	quit_properly(t_app *app)
{
	SDL_DestroyRenderer(app->sdl.ren);
	SDL_DestroyWindow(app->sdl.win);
	SDL_Quit();
}
