#include "wolf3d.h"

static void		move_forward(t_map *map, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(player->speed * player->x_v)
			+ (player->x_v > 0 ? player->max_dist : -player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->x_v > 0 ? -player->max_dist : player->max_dist);
	old_y = player->y;
	player->y += (int)(player->speed * player->y_v)
			+ (player->y_v > 0 ? player->max_dist : -player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->y_v > 0 ? -player->max_dist : player->max_dist);
}

static void		move_backward(t_map *map, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(-player->speed * player->x_v)
			+ (player->x_v > 0 ? -player->max_dist : player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->x_v > 0 ? player->max_dist : -player->max_dist);
	old_y = player->y;
	player->y += (int)(-player->speed * player->y_v)
			+ (player->y_v > 0 ? -player->max_dist : player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->y_v > 0 ? player->max_dist : -player->max_dist);
}

static void		move_left(t_map *map, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(-player->speed * player->y_v)
			+ (player->y_v > 0 ? -player->max_dist : player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->y_v > 0 ? player->max_dist : -player->max_dist);
	old_y = player->y;
	player->y += (int)(player->speed * player->x_v)
			+ (player->x_v > 0 ? player->max_dist : -player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->x_v > 0 ? -player->max_dist : player->max_dist);
}

static void		move_right(t_map *map, t_player *player)
{
	int		old_x;
	int		old_y;

	old_x = player->x;
	player->x += (int)(player->speed * player->y_v)
			+ (player->y_v > 0 ? player->max_dist : -player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->x = old_x;
	else
		player->x += (player->y_v > 0 ? -player->max_dist : player->max_dist);
	old_y = player->y;
	player->y += (int)(-player->speed * player->x_v)
			+ (player->x_v > 0 ? -player->max_dist : player->max_dist);
	if (map->nodes[player->y / 64][player->x / 64].collidable)
		player->y = old_y;
	else
		player->y += (player->x_v > 0 ? player->max_dist : -player->max_dist);
}

void			player_movement(t_map *map, const Uint8 *key, t_player *player)
{
	if (key[SDL_SCANCODE_W])
		move_forward(map, player);
	if (key[SDL_SCANCODE_S])
		move_backward(map, player);
	if (key[SDL_SCANCODE_A])
		move_right(map, player);
	if (key[SDL_SCANCODE_D])
		move_left(map, player);
}
