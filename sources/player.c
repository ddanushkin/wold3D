#include "wolf3d.h"

void	player_init(t_player *player)
{
	player->direction = 270;
	player->speed = 5;
	player->x_v = 0;
	player->y_v = 0;
}

void	player_rotate(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		(player->direction -= player->speed) < 0 ? player->direction = 357 : 0;
	if (state[SDL_SCANCODE_RIGHT])
		(player->direction += player->speed)  > 359 ? player->direction = 4 : 0;
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT])
		printf("player_dir - %d\n", player->direction);
}

void	player_move(t_map *map, const Uint8	*state, t_player *player)
{
	int new_x;
	int new_y;

	player_rotate(player, state);
	player->x_v = cos(player->direction * M_PI / 180.0);
	player->y_v = sin(player->direction * M_PI / 180.0);

	if(state[SDL_SCANCODE_W])
	{
		new_x = player->x + (int)(player->speed * player->x_v);
		new_y = player->y + (int)(player->speed * player->y_v);
		if(!map->nodes[new_y / 64][new_x / 64].collidable)
		{
			player->x = new_x;
			player->y = new_y;
		}
	}

	if(state[SDL_SCANCODE_S])
	{
		new_x = player->x - (int)(player->speed * player->x_v);
		new_y = player->y - (int)(player->speed * player->y_v);
		if(!map->nodes[new_y / 64][new_x / 64].collidable)
		{
			player->x = new_x;
			player->y = new_y;
		}
	}

}
