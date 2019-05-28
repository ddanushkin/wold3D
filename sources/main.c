/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:01:11 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/28 18:33:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int	check_for_quit(t_sdl *sdl, const Uint8 *key)
{
	if (SDL_PollEvent(&sdl->event) && sdl->event.type == SDL_QUIT)
		return (1);
	if (key[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}

void		start_the_game(t_app *app)
{
	const Uint8	*key;
	t_time		time;

	init_time(&time);
	key = SDL_GetKeyboardState(NULL);
	while (1)
	{
		if (check_for_quit(app->sdl, key) == 1)
			break ;
		update_time(&time, app);
		create_field_of_view(app);
		keyboard_input(app, key, time.frame);

		if (app->player->shooting == 0 && app->player->reloading == 0)
			gun_idle(app->sdl, app->player, time.frame);
		if (app->player->shooting != 0 && app->player->reloading == 0)
		{
			app->player->anim_is_done = 0;
			gun_shoot(app->sdl, app->player, time.frame);
		}
		if (app->player->reloading != 0 && app->player->shooting == 0)
		{
			app->player->anim_is_done = 0;
			gun_change(app->sdl, app->player, 0, time.frame);
		}

		create_hud(app->sdl, app->player);
		draw_face(app->sdl, app->player, time.frame);
		SDL_RenderPresent(app->sdl->renderer);
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
