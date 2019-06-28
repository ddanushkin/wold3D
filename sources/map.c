#include "wolf3d.h"

void		map_init(int fd, t_map *map)
{
	char	*line;
	char	**data;
	int		i;

	i = 0;
	line = NULL;
	ft_gnl(fd, &line);
	data = ft_strsplit(line, ' ');
	ft_strdel(&line);
	map->rows = ft_atoi(data[0]);
	map->cols = ft_atoi(data[1]);
	ft_delarr(data);
	map->nodes = (t_node **)malloc(sizeof(t_node *) * map->rows);
	map->objects = (t_node **)malloc(sizeof(t_node *) * map->rows * map->cols);
	map->objects_count = 0;
	while (i < map->rows)
		map->nodes[i++] = (t_node *)malloc(sizeof(t_node) * map->cols);
	map->doors = (t_node **)malloc(sizeof(t_node *) * 100);
	map->doors_count = 0;
}

SDL_Surface	*load_surf(char *dir, char *name, char *add)
{
	char	file_path[50];

	ft_strcpy(file_path, "../resources/");
	ft_strcat(file_path, dir);
	ft_strcat(file_path, name);
	ft_strcat(file_path, add);
	ft_strcat(file_path, ".bmp");
	return (SDL_LoadBMP(file_path));
}

void		fill_row(t_map *map, char **data, int row, t_player *player)
{
	int		col;
	t_node	*node;
	
	col = 0;
	while (col < map->cols)
	{
		node = &map->nodes[row][col];
		node->visible = false;
		node->collidable = 0;
		node->type = MAP_TYPE_EMPTY;
		node->x = col;
		node->y = row;
		if (*data[col] >= '1' && *data[col] <= '9')
		{
			node->type = MAP_TYPE_WALL;
			node->texture[0] = load_surf("walls/", data[col], "_n");
			node->texture[1] = load_surf("walls/", data[col], "_s");
			node->texture[2] = load_surf("walls/", data[col], "_w");
			node->texture[3] = load_surf("walls/", data[col], "_e");
			node->collidable = true;
		}
		else if (*data[col] >= 'X' && *data[col] <= 'Z')
		{
			node->type = MAP_TYPE_INTERIOR;
			node->center.y = row * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			node->center.x = col * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			node->texture[0] = load_surf("interior/", data[col], "");
			node->collidable = true;
			map->objects[map->objects_count++] = node;
		}
		else if (*data[col] == 'D')
		{
			node->type = MAP_TYPE_DOOR;
			node->texture[0] = load_surf("doors/", data[col], "1");
			node->door_frame = 0;
			node->door_closing = false;
			node->door_opening = false;
			node->last_open = 0;
			node->center.x = col * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			node->center.y = row * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			node->collidable = true;
			map->doors[map->doors_count++] = node;
		}
		else if (*data[col] == 'P')
		{
			player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
		}
		col++;
	}
}

void		map_read(int fd, t_map *map, t_player *player)
{
	char	*line;
	char	**data;
	int		i;

	i = 0;
	line = NULL;
	map_init(fd, map);
	while (ft_gnl(fd, &line))
	{
		data = ft_strsplit(line, ' ');
		fill_row(map, data, i++, player);
		ft_strdel(&line);
		ft_delarr(data);
	}
	close(fd);
	ft_strdel(&line);
}
