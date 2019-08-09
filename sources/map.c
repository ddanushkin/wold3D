#include "wolf3d.h"

int		valid_characters(char *c)
{
	if (*c == 'W' || *c == 'I' || *c == 'D' || *c == 'P')
		if (ft_isdigit(*(c + 1)) &&
		((*(c + 2) == '\t' || *(c + 2) == '\n' || *(c + 2) == '\0')))
			return (1);
	return (0);
}

void	change_row_length(char *str, t_row *row, t_map *map)
{
	str += 2;
	row->current_length++;
	if (*str == '\n')
	{
		map->rows++;
		if (row->max_length < row->current_length)
			row->max_length = row->current_length;
		row->current_length = 0;
	}
}

int		map_validation(char *str, t_map *map)
{
	t_row	row;

	row.current_length = 0;
	row.max_length = 0;
	map->rows = 1;
	while (*str)
	{
		if (valid_characters(str))
		{
			change_row_length(str, &row, map);
			str += 3;
		}
		else if (*str == '.')
		{
			row.current_length++;
			str += 2;
		}
		else
		{
			ft_error("Bad map. GoodBye");
			return (0);
		}
	}
	map->cols = row.max_length;
	return (1);
}

char	*write_map_to_string(int fd)
{
	char	*read_string;
	char	*level;

	read_string = ft_strnew(999);
	read(fd, read_string, 999);
	level = ft_strdup(read_string);
	free(read_string);
	return (level);
}

char	*map_extend(char *old_level, t_map *map)
{
	char	*new_level;
	char	*start;

	new_level = ft_strnew(ft_strlen(old_level) + (map->rows * 3));
	start = new_level;
	while (*old_level)
	{
		if (*old_level == '\n')
		{
			*new_level++ = '\t';
			*new_level++ = 'W';
			*new_level++ = '1';
			*new_level++ = *old_level++;
		}
		else
			*new_level++ = *old_level++;
	}
	*new_level++ = '\t';
	*new_level++ = 'W';
	*new_level++ = '1';
	*new_level = '\0';
	map->rows++;
	map->cols++;
	return (start);
}

int		map_read(int fd, t_app *app)
{
	int		i;
	char	**data;
	char	**lines;
	char	*level;

	i = 0;
	level = write_map_to_string(fd);
	if (!map_validation(level, app->map))
	{
		close(fd);
		return (0);
	}
	level = map_extend(level, app->map);
	lines = ft_strsplit(level, '\n');
	map_init(app->map);
	while (*lines)
	{
		data = ft_strsplit(*lines, '\t');
		fill_row(app, data, i++);
		ft_strdel(lines++);
		ft_delarr(data);
	}
	close(fd);
	free(level);
	return (1);
}

void	map_init(t_map *map)
{
	int		i;

	i = 0;
	map->doors_count = 0;
	map->objects_count = 0;
	map->nodes = (t_node **)malloc(sizeof(t_node *) * map->rows);
	map->objects = (t_node **)malloc(sizeof(t_node *) * map->objects_count);
	while (i < map->rows)
		map->nodes[i++] = (t_node *)malloc(sizeof(t_node) * map->cols);
	map->doors = (t_node **)malloc(sizeof(t_node *) * map->doors_count);
}

void	node_reset(t_node *node, int row, int col)
{
	node->visible = false;
	node->collidable = false;
	node->type = MAP_TYPE_EMPTY;
	node->x = col;
	node->y = row;
}

void	fill_row(t_app *app, char **data, int row)
{
	int		col;
	t_node	*node;
	int		index;
	char	type;

	col = 0;
	while (col < app->map->cols)
	{
		node = &app->map->nodes[row][col];
		node_reset(node, row, col);
		index = ft_atoi((data[col] + 1)) - 1;
		type = *data[col];
		if (type == 'W')
			map_type_wall(app, node, index);
		else if (type == 'I')
			map_type_interior(app, node, index);
		else if (type == 'D')
			map_type_door(app, node, index);
		else if (type == 'P')
		{
			app->player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
			app->player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
		}
		col++;
	}
}
