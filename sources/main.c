/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:01:11 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/03 15:51:52 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static	int	check_for_quit(t_sdl *sdl, const Uint8 *key)
{
	if (SDL_PollEvent(&sdl->event) && sdl->event.type == SDL_QUIT)
		return (1);
	if (key[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}

void		start_the_game(t_app *wolf)
{
	const Uint8	*key;
	t_time		time;
	Uint64		start;
	Uint64		end;
	float		elapsed;

	init_time(&time);
	key = SDL_GetKeyboardState(NULL);
	while (1)
	{
		start = SDL_GetPerformanceCounter();
		if (check_for_quit(wolf->sdl, key) == 1)
			break ;
		update_time(&time, wolf);
		keyboard_input(wolf, key, time.frame);
		create_field_of_view(wolf);
		redraw(wolf->sdl, wolf->player, &time);
		end = SDL_GetPerformanceCounter();
		elapsed = (float)(end - start) / SDL_GetPerformanceFrequency() * 1000.0;
		SDL_Delay((Uint32)(elapsed - 16.6666)); //60 FPS
	}
}

void		load_level(t_app *wolf)
{
	int		fd;

	wolf->player->cur_level = "1";
	if ((fd = open("../levels/1.wolf3d", O_RDONLY)) != -1)
		map_read(fd, wolf->map, wolf->player);
	else
		ft_error("Map path error.");
}

int			main(void)
{
	t_app	wolf;

	init(&wolf);
	player_init(wolf.sdl, wolf.player);
	load_level(&wolf);
	start_the_game(&wolf);
	quit_properly(&wolf);
	return (0);
}
