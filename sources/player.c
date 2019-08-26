#include "wolf3d.h"

void		draw_face(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect	area;
	int			cur_frame;

	area.h = 116;
	area.w = 96;
	area.x = sdl->half_width - area.w + 5;
	area.y = sdl->height - 130;
	cur_frame = (long)delta % 4 * 7;
	if (cur_frame == 14)
		cur_frame = 0;
	if (player->health < 85)
		cur_frame += 6 - ((player->health) / 14) % 7;
	SDL_RenderCopy(sdl->renderer, player->faces[cur_frame], NULL, &area);
}

void		player_init(t_sdl *sdl, t_player *player)
{
	ft_bzero(player, sizeof(t_player));
	player->direction = 90;
	player->lives = 99;
	player->health = 100;
	player->max_dist = 25;
	player->x_v = cos(player->direction * M_PI_180);
	player->y_v = sin(player->direction * M_PI_180);
	load_faces(sdl, player);
	load_sounds(player);
	load_weapons(sdl, player);
}

void		update_sound(const Uint8 *key, t_sfx *sfx)
{
	if (key[SDL_SCANCODE_M])
	{
		if (!Mix_PlayingMusic())
			Mix_PlayMusic(sfx->background, -1);
		else if (Mix_PausedMusic())
			Mix_ResumeMusic();
	}
	if (key[SDL_SCANCODE_P])
		Mix_PauseMusic();
}

void		place_player(t_player *player, t_node *node, int row, int col)
{
	player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
	player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
	map_type_empty(node);
}

void		fix_direction(t_app *app)
{
	if (app->player->direction > 359)
		app->player->direction = 0;
	if (app->player->direction < 0)
		app->player->direction = 359;
}
