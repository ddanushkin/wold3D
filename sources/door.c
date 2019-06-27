#include "wolf3d.h"

static void		auto_close(t_node		*door_node, t_player *player, float frame)
{
	int dist_to_player;

	if (door_node->last_open != 0)
	{
		dist_to_player = sqrt(ft_powi(door_node->center.x - player->x, 2) + ft_powi(door_node->center.y - player->y, 2));
		printf("dist_to_player - %d\n", dist_to_player);
	}
	if (!door_node->door_opening && door_node->door_frame == 64 && frame - door_node->last_open >= 5 && dist_to_player >= 60)
	{
		door_move_sound();
		door_node->last_open = 0;
		door_node->door_closing = true;
	}
}

void		update_doors(t_map *map, t_player *player, float frame)
{
	t_node		*door_node;
	int			i;

	i = 0;
	while (i < map->doors_count)
	{
		door_node = map->doors[i];
		auto_close(door_node, player, frame);
		door_node->door_frame -= door_node->door_closing ? 1 : 0;
		door_node->door_frame += door_node->door_opening ? 1 : 0;
		if (door_node->door_frame == 59 && door_node->door_opening)
			door_close_open_sound();
		if (door_node->door_frame == 5 && door_node->door_closing)
			door_close_open_sound();
		if (door_node->door_frame > 64)
		{
			door_node->door_opening = false;
			door_node->last_open = frame;
			door_node->door_frame = 64;
		}
		if (door_node->door_frame < 0)
		{
			door_node->door_closing = false;
			door_node->door_frame = 0;
		}
		door_node->collidable = door_node->door_frame > 32 ? false : true;
		i++;
	}
}

void			door_interaction(t_app *app, float frame)
{
	t_ipoint	near_coord;
	t_node		*near_node;

	app->player->last_space = frame;
	near_coord.x = app->player->x + cos(app->player->direction * M_PI_180) * 64;
	near_coord.y = app->player->y + sin(app->player->direction * M_PI_180) * 64;
	near_node = &app->map->nodes[near_coord.y / 64][near_coord.x / 64];
	if (near_node->type == MAP_TYPE_DOOR)
	{
		door_move_sound();
		if (!near_node->door_closing && !near_node->door_opening)
		{
			if (near_node->door_frame == 0)
				near_node->door_opening = true;
			if (near_node->door_frame == 64)
				near_node->door_closing = true;
		}
		else
		{
			near_node->door_opening = !near_node->door_opening;
			near_node->door_closing = !near_node->door_closing;
		}
	}
}

void 		door_move_sound()
{
	Mix_Chunk *clip;
	clip = Mix_LoadWAV("../resources/sounds/door_move.wav");
	Mix_PlayChannel(2, clip, 0);
}

void 		door_close_open_sound()
{
	Mix_Chunk *clip;
	clip = Mix_LoadWAV("../resources/sounds/door_open_close.wav");
	Mix_PlayChannel(3, clip, 0);
}