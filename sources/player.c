#include "wolf3d.h"

void	player_init(t_player *player)
{
	player->a = 180;
	player->speed = 1;
	player->x_v = 0;
	player->y_v = 0;
}

void	player_vel(t_player *player, SDL_Event *event, const Uint8 *state)
{
	if(state[SDL_SCANCODE_LEFT] == 1)
		player->x_v = -1;
	else if(state[SDL_SCANCODE_LEFT] == 0 && player->x_v < 0)
		player->x_v = 0;
	if(state[SDL_SCANCODE_RIGHT])
		player->x_v = 1;
	else if(state[SDL_SCANCODE_RIGHT] == 0 && player->x_v > 0)
		player->x_v = 0;
	if(state[SDL_SCANCODE_UP])
		player->y_v = -1;
	else if(state[SDL_SCANCODE_UP] == 0 && player->y_v < 0)
		player->y_v = 0;
	if(state[SDL_SCANCODE_DOWN])
		player->y_v = 1;
	else if(state[SDL_SCANCODE_DOWN] == 0 && player->y_v > 0)
		player->y_v = 0;
}

void	player_move(t_player *player)
{
	player->x += player->speed * player->x_v;
	player->y += player->speed * player->y_v;
}
