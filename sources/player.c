#include "wolf3d.h"

void	player_init(t_player *player)
{
	player->a = 180;
	player->speed = 1;
	player->x_v = 0;
	player->y_v = 0;
}

void	player_vel(t_player *player, const Uint8 *state)
{
	if(state[SDL_SCANCODE_A])
		player->x_v = (player->x_v < 0) ? 0 : -1;
	else if(state[SDL_SCANCODE_D])
		player->x_v = (player->x_v > 0) ? 0 : 1;
	else if(state[SDL_SCANCODE_W])
		player->y_v = (player->y_v < 0) ? 0 : -1;
	else if (state[SDL_SCANCODE_S])
		player->y_v = (player->y_v > 0) ? 0 : 1;
	//hren
}

void	player_move(t_player *player)
{
	player->x += player->speed * player->x_v;
	player->y += player->speed * player->y_v;
}