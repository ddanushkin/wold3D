/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:01:11 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/25 21:06:00 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		player_debug(const Uint8 *key, t_player *player)
{
	if (key[SDL_SCANCODE_EQUALS] && player->health < 100)
		player->health++;
	if (key[SDL_SCANCODE_MINUS] && player->health > 0)
	{
		player->health--;
		if (player->health == 0)
			Mix_PlayChannel(-1, player->fx_die, 0);
	}
	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
		printf("%d\n", 6 - ((player->health) / 14) % 7);
}

void		start_the_game(t_app *wolf)
{
	const Uint8	*key;
	t_time		time;
	Uint64		start;
	Uint64		end;
	float		elapsed;
	t_node		*door_node;

	door_node = &wolf->map->nodes[13][6];
	init_time(&time);
	key = SDL_GetKeyboardState(NULL);
	while (1)
	{
		start = SDL_GetPerformanceCounter();
		if (check_for_quit(wolf->sdl, key) == 1)
			break ;
		update_time(&time, wolf);
		player_debug(key, wolf->player);
		keyboard_input(wolf, key, time.frame);
		if (door_node->door_closing)
			door_node->door_frame--;
		if (door_node->door_opening)
			door_node->door_frame++;
		if (door_node->door_frame > 64)
		{
			door_node->collidable = false;
			door_node->door_opening = false;
			door_node->door_closing = false;
			door_node->door_frame = 64;
		}
		if (door_node->door_frame < 0)
		{
			door_node->collidable = true;
			door_node->door_opening = false;
			door_node->door_closing = false;
			door_node->door_frame = 0;
		}
		create_field_of_view(wolf);
		redraw(wolf->sdl, wolf->player, &time);
		end = SDL_GetPerformanceCounter();
		elapsed = (float)(end - start) / SDL_GetPerformanceFrequency() * 1000.0;
		SDL_Delay((Uint32)(elapsed - 16.6666));
	}
}

void		load_level(t_app *wolf, int level)
{
	int		fd;
	char	*level_char;
	char	level_path[50];

	ft_strdel(&wolf->player->cur_level);
	level_char = ft_itoa(level);
	wolf->player->cur_level = level_char;
	ft_strcpy(level_path, "../levels/");
	ft_strcat(level_path, level_char);
	ft_strcat(level_path, ".wolf3d");
	if ((fd = open(level_path, O_RDONLY)) != -1)
		map_read(fd, wolf->map, wolf->player);
	else
		ft_error("Map path error.");
}

int			main(void)
{
	t_app	wolf;

	init(&wolf);
	player_init(wolf.sdl, wolf.player);
	load_level(&wolf, 1);
	start_the_game(&wolf);
	quit_properly(&wolf);
	return (0);
}
