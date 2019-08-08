#include "wolf3d.h"

int		valid_characters(char *c)
{
	if (*c == 'W' || *c == 'I' || *c == 'D' || *c == 'P')
		if (isdigit(*(c + 1)))
			return (1);
	return (0);
}

int		map_validation(char *str, t_app *app)
{
	int 	max_col;
	int 	cur_col;

	cur_col = 0;
	max_col = 0;
	app->map->doors_count = 0;
	app->map->objects_count = 0;
	app->map->rows = 1;
	while (*str)
	{
		if (valid_characters(str))
		{
			str += 2;
			cur_col += 1;
			if ((*str != '\t' && *str != '\n' && *str != '\0'))
			{
				ft_error("Bad map. GoodBye");
				return (0);
			}
		}
		else if (*str == '.')
		{
			cur_col++;
			if (*(str + 1) != '\t')
			{
				ft_error("Bad map. GoodBye");
				break;
			}
		}
		if (* str == '\n')
		{
			app->map->rows++;
			if (max_col < cur_col)
				max_col = cur_col;
			cur_col = 0;
		}
		str++;
	}
	app->map->cols = max_col;
	return (1);
}

char	*create_line(int fd)
{
	char	*line;
	char 	*level;
	size_t	count;
	char 	*lvl;
	int res;

	line = NULL;
	lvl = NULL;
	level = NULL;

	level = ft_strnew(999);
	res = read(fd, level, 999);

	count = 0;
/*	char *check;
	int res;
	check = NULL;
	level = ft_strnew(10);
	check = ft_strnew(10);
	count = 0;
	line = NULL;
	res = read(fd, level, 10);
	level[res] = '\0';*/

/*	int res;
	int final;

	final = 0;
	level = ft_strnew(5);
	//res = read(fd, level, 5);
	while (1)
	{
		res = read(fd, level, 5);
		final += res;
		if (res == 0)
			break;
	}*/


	/*while (ft_gnl(fd, &line))
	{
		count += ft_strlen(line);
		ft_strsplit(line, '\t');
		ft_strcat(level, line);
		ft_strcat(level, "\n");
		ft_strdel(&line);
	}
	level[count + 2] = 0;
	//lvl = ft_strnew(count);
	//ft_strcpy(lvl, level);*/
	return (level);
}

void		map_read(int fd, t_app *app)
{
	char	**data;
	char 	**lines;
	int		i;
	char 	*level;

	i = 0;
	level = create_line(fd);
	if (!map_validation(level, app))
		ft_error("map error!");
	map_init(app->map);
	lines = ft_strsplit(level, '\n');
	while (*lines)
	{
		data = ft_strsplit(*lines, '\t');
		fill_row(app, data, i++);
		ft_strdel(lines);
		ft_delarr(data);
		lines++;
	}
	close(fd);
}

void		map_init(t_map *map)
{
	int		i;

	i = 0;
	map->nodes = (t_node **)malloc(sizeof(t_node *) * map->rows);
	map->objects = (t_node **)malloc(sizeof(t_node *) * map->objects_count);
	map->objects_count = 0;
	while (i < map->rows)
		map->nodes[i++] = (t_node *)malloc(sizeof(t_node) * map->cols);
	map->doors = (t_node **)malloc(sizeof(t_node *) * map->doors_count);
	map->doors_count = 0;
}

void		node_reset(t_node *node, int row, int col)
{
	node->visible = false;
	node->collidable = false;
	node->type = MAP_TYPE_EMPTY;
	node->x = col;
	node->y = row;
}

void		fill_row(t_app *app, char **data, int row)
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
