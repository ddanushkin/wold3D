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

void		fill_row(t_app *app, char **data, int row)
{
	int		col;
	t_node	*node;
	char type;
	int index;

	col = 0;
	while (col < app->map->cols)
	{
		type = *data[col];
		index = ft_atoi((data[col] + 1)) - 1;
		node = &app->map->nodes[row][col];
		node->visible = false;
		node->collidable = false;
		node->type = MAP_TYPE_EMPTY;
		node->x = col;
		node->y = row;
		if (type == 'W')
			map_type_wall(app, node, index);
		else if (type == 'I')
			map_type_interior(app, node, data[col]);
		else if (type == 'D')
			map_type_door(app, node, data[col]);
		else if (type == 'P')
		{
			app->player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
			app->player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
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

void		map_read(int fd, t_app *app)
{
	char	*line;
	char	**data;
	int		i;

	i = 0;
	line = NULL;
	map_init(fd, app->map);
	while (ft_gnl(fd, &line))
	{
		data = ft_strsplit(line, '\t');
		fill_row(app, data, i++);
		ft_strdel(&line);
		ft_delarr(data);
	}
	//scaled_number(map);
	close(fd);
	ft_strdel(&line);
}
