/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:01:11 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/22 17:58:32 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	update_picture(t_sdl *sdl)
{
	SDL_UpdateTexture(sdl->texture, NULL, sdl->pixels, sdl->width * sizeof(Uint32));
	SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
	SDL_RenderPresent(sdl->renderer);
}

static int	check_for_quit(t_app *app, Uint8 *key)
{
	if (SDL_PollEvent(&app->sdl->event) && app->sdl->event.type == SDL_QUIT)
		return (1);
	if (key[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}

void		start_game(t_app *app)
{
	const Uint8	*key;
	t_time		time;

	init_time(&time);
	key = SDL_GetKeyboardState(NULL);
	while (1)
	{
		update_time(&time, app);
		if (check_for_quit(app, key) == 1)
			break ;
		time.frame += time.delta;
		if (time.frame >= 0.25)
		{
			time.frame = 0; //  для движи
		}
		create_field_of_view(app);
		move_player(app->map, key, app->player);
		update_picture(app->sdl);
	}
}

void		load_level(t_app *wolf)
{
	int		fd;

	if ((fd = open("../levels/1.wolf3d", O_RDONLY)) != -1)
		map_read(fd, wolf->map, wolf->player);
	else
		ft_error("Map path error.");
}

int			main(void)
{
	t_app	wolf;

	init(&wolf);
	player_init(wolf.player);
	load_level(&wolf);
	start_game(&wolf);
	quit_properly(&wolf);
	return (0);
}
