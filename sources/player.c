#include "wolf3d.h"

void	player_init(t_player *player)
{
	player->direction = 90;
	player->x_vector = -1;
	player->y_vector = 0;
	player->x_plane = 0;
	player->y_plane = 0.66;
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
	{
		player->direction++;
		if (player->direction > 360)
			player->direction = 0;
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		player->direction--;
		if (player->direction < 0)
			player->direction = 360;
	}
}

void	player_move(t_player *player)
{
	player->x += player->speed * player->x_v;
	player->y += player->speed * player->y_v;
//	if ((player->direction >= 0 && player->direction <= 90) || player->direction == 360)
//	{
//		player->x += player->speed * player->x_v;
//		player->y -= player->speed * player->y_v;
//	}
//	else if ((player->direction >= 91 && player->direction <= 180))
//	{
//		player->x -= player->speed * player->x_v;
//		player->y -= player->speed * player->y_v;
//	}
//	else if ((player->direction >= 181 && player->direction <= 270))
//	{
//		player->x += player->speed * player->x_v;
//		player->y += player->speed * player->y_v;
//	}
//	else if ((player->direction >= 271 && player->direction <= 359))
//	{
//		player->x -= player->speed * player->x_v;
//		player->y += player->speed * player->y_v;
//	}
}
