/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 17:19:36 by lglover           #+#    #+#             */
/*   Updated: 2019/04/29 15:06:09 by ndremora         ###   ########.fr       */
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

void	read_map(t_map *map)
{
	char	*line;
	char	**data;
	int		res;
	int 	fd;

	fd = open("../levels/1.wolf3d", O_RDONLY);
	line = NULL;

	ft_gnl(fd, &line);
	data = ft_strsplit(line, ' ');
	map->rows = ft_atoi(data[0]);
	map->cols = ft_atoi(data[1]);
	ft_delarr(data);
	while ((res = ft_gnl(fd, &line)) > 0)
	{
		ft_strdel(&line);
	}
	close(fd);
	ft_strdel(&line);
}

int		main(void) {
	t_app		app;

	init(&app.sdl, &app.player);
	read_map(&app.map);
	update(&app.sdl, &app.player);
	SDL_DestroyRenderer(app.sdl.ren);
	SDL_DestroyWindow(app.sdl.win);
	SDL_Quit();
	return (0);
}
