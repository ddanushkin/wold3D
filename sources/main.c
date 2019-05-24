/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:01:11 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/24 19:08:53 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	update_picture(t_sdl *sdl, t_player *player, float delta)
{
	SDL_UpdateTexture(sdl->texture, NULL, sdl->pixels, sdl->width * sizeof(Uint32));
	SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
	create_hud(sdl, player);
	draw_face(sdl, player, delta);
	SDL_RenderPresent(sdl->renderer);
}

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
	float		flag;

	flag = 0;
	init_time(&time);
	key = SDL_GetKeyboardState(NULL);
	while (1)
	{
		if (check_for_quit(app->sdl, key) == 1)
			break ;
		update_time(&time, app);
		if (time.frame >= 2500)
		{
			flag = flag ? 0 : 1;
			time.frame = 0;
		}
		create_field_of_view(app);
		keyboard_input(app->map, key, app->player);
		update_picture(app->sdl, app->player, flag);
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
