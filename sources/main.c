/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 17:19:36 by lglover           #+#    #+#             */
/*   Updated: 2019/04/29 16:15:49 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		update(t_sdl *sdl, t_player *player)
{
	const Uint8	*state;

	while (1)
	{
		SDL_SetRenderDrawColor(sdl->ren, 0, 0, 0, 0);
		SDL_RenderClear(sdl->ren);
		if (SDL_PollEvent(&sdl->event) && sdl->event.type == SDL_QUIT)
			break;
		state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
			break;
		if (state[SDL_SCANCODE_LEFT])
			player->x++;
		if (state[SDL_SCANCODE_RIGHT])
			player->x--;
		if (state[SDL_SCANCODE_UP])
			player->y++;
		if (state[SDL_SCANCODE_DOWN])
			player->y--;
		SDL_SetRenderDrawColor(sdl->ren, 255, 0, 0, 255);
		SDL_RenderDrawPoint(sdl->ren, player->x, player->y);
		SDL_RenderPresent(sdl->ren);
	}
}

int		main(void)
{
	t_app		app;
	int fd;

	init(&app.sdl, &app.player);

	if ((fd = open("../levels/1.wolf3d", O_RDONLY)) != -1)
	{
		read_map(fd, &app.map);
		update(&app.sdl, &app.player);
	}
	else
		ft_error("Map path error.");
	quit_properly(&app);
	return (0);
}
