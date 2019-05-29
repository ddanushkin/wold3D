/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:01:11 by ndremora          #+#    #+#             */
/*   Updated: 2019/05/29 17:31:39 by lglover          ###   ########.fr       */
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

void		redraw(t_app *wolf, t_time *time)
{
	SDL_UpdateTexture(wolf->sdl->texture, NULL, wolf->sdl->pixels, wolf->sdl->width * sizeof(Uint32));
	SDL_RenderCopy(wolf->sdl->renderer, wolf->sdl->texture, NULL, NULL);

	if (wolf->player->shooting)
	{
		if (time->frame - wolf->player->anim_frame < wolf->player->weapon[wolf->player->cur_weapon].firerate)
		{
			if (!wolf->player->anim_is_done)
				gun_shoot(wolf->sdl, wolf->player, time->frame - wolf->player->anim_frame);
			else
				printf("SHOOT: WAIT\n");
		}
		else
		{
			wolf->player->shooting = 0;
			wolf->player->anim_frame = 0;
			wolf->player->weapon[wolf->player->cur_weapon].fired = 0;
			printf("SHOOT: DONE\n");
		}
	}

	if (wolf->player->changing)
	{
		if (time->frame - wolf->player->anim_frame < 1.8)
		{
			if (!wolf->player->anim_is_done)
				gun_change(wolf->sdl, wolf->player, 0, time->frame - wolf->player->anim_frame);
			else
				printf("CHANGE: WAIT\n");
		}
		else
		{
			wolf->player->changing = 0;
			wolf->player->anim_frame = 0;
			printf("CHANGE: DONE\n");
		}
	}

	if (wolf->player->reloading)
	{
		if (time->frame - wolf->player->anim_frame < 1.8)
		{
			if (!wolf->player->anim_is_done)
				gun_reload(wolf->sdl, wolf->player, time->frame - wolf->player->anim_frame);
			else
				printf("RELOAD: WAIT\n");
		}
		else
		{
			wolf->player->reloading = 0;
			wolf->player->anim_frame = 0;
			printf("CHANGE: DONE\n");
		}
	}

	if (wolf->player->anim_is_done)
		gun_idle(wolf->sdl, wolf->player, time->frame);

	create_hud(wolf->sdl, wolf->player);
	draw_face(wolf->sdl, wolf->player, time->frame);
	SDL_RenderPresent(wolf->sdl->renderer);
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
	keyboard_input(wolf, key, time.frame);
	while (1)
	{
		start = SDL_GetPerformanceCounter();

		if (check_for_quit(wolf->sdl, key) == 1)
			break ;
		update_time(&time, wolf);
		keyboard_input(wolf, key, time.frame);
		create_field_of_view(wolf);
		redraw(wolf, &time);

		end = SDL_GetPerformanceCounter();
		elapsed = (float)(end - start) / SDL_GetPerformanceFrequency() * 1000.0;
		SDL_Delay(elapsed - 16.6666); //60 FPS
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
