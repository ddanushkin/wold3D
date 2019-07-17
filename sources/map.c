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
		node->collidable = false;
		node->type = MAP_TYPE_EMPTY;
		node->x = col;
		node->y = row;
		if (*data[col] >= '1' && *data[col] <= '9')
			map_type_wall(node, data[col]);
		else if (*data[col] >= 'X' && *data[col] <= 'Z')
			map_type_interior(node, data[col], map);
		else if (*data[col] == 'D')
			map_type_door(node, data[col], map);
		else if (*data[col] == 'P')
		{
			player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
			player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
		}
		col++;
	}
}

void		scaled_number(t_map *map)
{
	int i;

	i = 0;
	map->true_doors = (t_node **)malloc(sizeof(t_node *) * map->doors_count);
	map->true_objects = (t_node **)malloc(sizeof(t_node *) * map->objects_count);
	while (i < map->doors_count)
	{
		map->true_doors[i] = map->doors[i];
		i++;
	}
	i = 0;
	while (i < map->objects_count)
	{
		map->true_objects[i] = map->objects[i];
		i++;
	}
	//free(map->doors);
	//free(map->objects);
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
	//scaled_number(map);
	close(fd);
	ft_strdel(&line);
}
