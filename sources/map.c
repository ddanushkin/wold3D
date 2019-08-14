#include "wolf3d.h"

char	*write_map_to_string(int fd)
{
	char	*read_string;

	read_string = ft_strnew(999999999);
	read(fd, read_string, 999999999);
	return (read_string);
}

int		map_read(int fd, t_app *app)
{
	int		i;
	char	**data;
	char	**lines;
	char	**tmp_lines;
	char	*level;

	i = 0;
	level = write_map_to_string(fd);
	close(fd);
	if (!map_count_rows(level, app->map))
		return (0);
	lines = ft_strsplit(level, '\n');
	tmp_lines = lines;
	map_init(app->map);
	map_fill(app);
	while (*lines)
	{
		data = ft_strsplit(*lines, '\t');
		fill_row(app, data, i++);
		lines++;
		ft_delarr(data);
	}
	ft_delarr(tmp_lines);
	free(level);
	return (1);
}

void	map_fill(t_app *app)
{
	int row;
	int col;

	row = 0;
	while (row < app->map->rows)
	{
		col = 0;
		while (col < app->map->cols)
		{
			node_reset(&app->map->nodes[row][col], row, col);
			map_type_wall(app, &app->map->nodes[row][col], 0);
			col++;
		}
		row++;
	}
}

void	map_init(t_map *map)
{
	int		i;

	map->nodes = (t_node **)malloc(sizeof(t_node *) * map->rows);
	i = 0;
	while (i < map->rows)
		map->nodes[i++] = (t_node *)malloc(sizeof(t_node) * map->cols);
	map->doors = (t_node **)malloc(sizeof(t_node *) * map->doors_count);
	map->objects = (t_node **)malloc(sizeof(t_node *) * map->objects_count);
	map->objects_count = 0;
	map->doors_count = 0;
}

void	fill_row(t_app *app, char **data, int row)
{
	int		col;
	t_node	*node;
	int		index;
	char	type;

	col = 0;
	while (col < app->map->cols - 1 && data[col])
	{
		node = &app->map->nodes[row + 1][col + 1];
		type = *data[col];
		index = ft_atoi((data[col] + 1)) - 1;
		if (type == 'W')
			map_type_wall(app, node, index);
		else if (type == 'I')
			map_type_interior(app, node, index);
		else if (type == 'D')
			map_type_door(app, node, index);
		else if (type == '.')
			map_type_empty(node);
		else if (type == 'P')
			place_player(app->player, node, row, col);
		col++;
	}
}
