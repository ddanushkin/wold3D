#include "wolf3d.h"

void		map_type_wall(t_app *app, t_node *node, char *data)
{
	int index;

	index = ft_atoi(data) - 1;
	node->type = MAP_TYPE_WALL;
	node->texture[0] = app->textures->walls[index];
	node->texture[1] = app->textures->walls[index + 1];
	node->texture[2] = app->textures->walls[index + 2];
	node->texture[3] = app->textures->walls[index + 3];
	node->collidable = true;
}

void		map_type_interior(t_app *app, t_node *node, char *data)
{
	node->type = MAP_TYPE_INTERIOR;
	node->texture[0] = app->textures->sprites[0];
	node->collidable = true;
	node->center.x = node->x * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	node->center.y = node->y * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	node->visible = true;
	app->map->objects[app->map->objects_count++] = node;
	node->screen_x = 0;
}

void		map_type_door(t_app *app, t_node *node, char *data)
{
	node->type = MAP_TYPE_DOOR;
	node->texture[0] = app->textures->doors[0];
	node->door_frame = 0;
	node->door_closing = false;
	node->door_opening = false;
	node->last_open = 0;
	node->center.x = node->x * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	node->center.y = node->y * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	node->collidable = true;
	app->map->doors[app->map->doors_count++] = node;
}
