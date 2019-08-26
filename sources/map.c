/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:25:49 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:55:43 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	map_init(app);
	while (*lines)
	{
		data = ft_strsplit(*lines++, '\t');
		fill_row(app, data, i++);
		ft_delarr(data);
	}
	ft_delarr(tmp_lines);
	free(level);
	if (!app->players_count)
		return (0);
	return (1);
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
			map_type_interior(app, node);
		else if (type == 'D')
			map_type_door(app, node);
		else if (type == '.')
			map_type_empty(node);
		else if (type == 'P')
			place_player(app, node, row + 1, col + 1);
		col++;
	}
}
