#include "wolf3d.h"

void		map_type_wall(t_node *node, char *data)
{
	node->type = MAP_TYPE_WALL;
	node->texture[0] = load_surf("walls/", data, "_n");
	node->texture[1] = load_surf("walls/", data, "_s");
	node->texture[2] = load_surf("walls/", data, "_w");
	node->texture[3] = load_surf("walls/", data, "_e");
	node->collidable = true;
}

void		map_type_interior(t_node *node, char *data, t_map *map)
{
	node->type = MAP_TYPE_INTERIOR;
	node->texture[0] = load_surf("interior/", data, "");
	node->collidable = true;
	node->center.x = node->x * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	node->center.y = node->y * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	map->objects[map->objects_count++] = node;
}

void		map_type_door(t_node *node, char *data, t_map *map)
{
	node->type = MAP_TYPE_DOOR;
	node->texture[0] = load_surf("doors/", data, "1");
	node->door_frame = 0;
	node->door_closing = false;
	node->door_opening = false;
	node->last_open = 0;
	node->center.x = node->x * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	node->center.y = node->y * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
	node->collidable = true;
	map->doors[map->doors_count++] = node;
}
