#include "wolf3d.h"

void	player_is_collising(t_map *map, t_player *player)
{
	if (player->x_v < 0)
	{
		if(map->nodes[player->y / 64][(player->x - player->speed) / 64].collidable)
			player->x_v = 0;
	}
	if (player->x_v > 0)
	{
		if(map->nodes[player->y / 64][(player->x + player->speed) / 64].collidable)
			player->x_v = 0;
	}
	if (player->y_v < 0)
	{
		if(map->nodes[(player->y - player->speed) / 64][player->x / 64].collidable)
			player->y_v = 0;
	}
	if (player->y_v > 0)
	{
		if(map->nodes[(player->y + player->speed) / 64][player->x / 64].collidable)
			player->y_v = 0;
	}
}

void	player_init(t_player *player)
{
	player->direction = 90;
	player->speed = 5;
	player->x_v = 0;
	player->y_v = 0;
}

void	player_vel(t_player *player, const Uint8 *state)
{
	if(state[SDL_SCANCODE_A])
		player->x_v = -1;
	else if(player->x_v < 0)
		player->x_v = 0;
	if(state[SDL_SCANCODE_D])
		player->x_v = 1;
	else if(player->x_v > 0)
		player->x_v = 0;
	if(state[SDL_SCANCODE_W])
		player->y_v = -1;
	else if(player->y_v < 0)
		player->y_v = 0;
	if(state[SDL_SCANCODE_S])
		player->y_v = 1;
	else if(player->y_v > 0)
		player->y_v = 0;
	if (state[SDL_SCANCODE_LEFT])
		--player->direction < 0 ? player->direction = 359 : 0;
	if (state[SDL_SCANCODE_RIGHT])
		++player->direction > 359 ? player->direction = 0 : 0;
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT])
		printf("player_dir - %d\n", player->direction);
}

void	player_move(t_player *player)
{
	player->x += player->speed * player->x_v;
	player->y += player->speed * player->y_v;
/*	if ((player->direction >= 0 && player->direction <= 90) || player->direction == 360)
	{
		player->x += player->speed * player->x_v;
		player->y -= player->speed * player->y_v;
	}
	else if ((player->direction >= 91 && player->direction <= 180))
	{
		player->x -= player->speed * player->x_v;
		player->y -= player->speed * player->y_v;
	}
	else if ((player->direction >= 181 && player->direction <= 270))
	{
		player->x += player->speed * player->x_v;
		player->y += player->speed * player->y_v;
	}
	else if ((player->direction >= 271 && player->direction <= 359))
	{
		player->x -= player->speed * player->x_v;
		player->y += player->speed * player->y_v;
	}*/
}
