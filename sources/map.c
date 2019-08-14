#include "wolf3d.h"

int		valid_characters(char *c)
{
	if (*c == '.' && *(c + 1) == '\0')
		return (1);
	if (*c == 'W' || *c == 'I' || *c == 'D' || *c == 'P')
		if (ft_isdigit(*(c + 1)) && *(c + 2) == '\0')
			return 1;
	return (0);
}

char	*write_map_to_string(int fd)
{
	char	*read_string;

	read_string = ft_strnew(999);
	read(fd, read_string, 999);
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

void	place_player(t_player *player, int row, int col)
{
	player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
	player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE * 0.5);
}

void	fill_row(t_app *app, char **data, int row)
{
	int		col;
	t_node	*node;
	int		index;
	char	type;

	col = 0;
	while (col < app->map->cols - 1)
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
		{
			place_player(app->player, row, col);
			map_type_empty(node);
		}
		col++;
		if (!data[col])
			break ;
	}
}

int	count_words(char const *s, char c)
{
	int	cnt;

	cnt = 0;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == '\0'))
		{
			cnt++;
		}
		s++;
	}
	return (cnt);
}

int		map_count_cols(char *row, t_map *map)
{
	char	**cols;
	char	**tmp_cols;

	if (map->cols < count_words(row, '\t') + 2)
		map->cols = count_words(row, '\t') + 2;
	cols = ft_strsplit(row, '\t');
	tmp_cols = cols;
	while (*cols)
	{
		if (!valid_characters(*cols))
		{
			ft_delarr(tmp_cols);
			return (0);
		}
		if (*cols[0] == 'D')
			map->doors_count++;
		if (*cols[0] == 'I')
			map->objects_count++;
		cols++;
	}
	ft_delarr(tmp_cols);
	return (1);
}

int		map_count_rows(char *str, t_map *map)
{
	char	**rows;
	char	**tmp_rows;

	map->rows = count_words(str, '\n') + 2;
	map->cols = 0;
	rows = ft_strsplit(str, '\n');
	tmp_rows = rows;
	while (*rows)
	{
		if (!map_count_cols(*rows, map))
		{
			ft_delarr(tmp_rows);
			ft_error("Bad map. GoodBye");
			return (0);
		}
		rows++;
	}
	ft_delarr(tmp_rows);
	return (1);
}