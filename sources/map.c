/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 15:56:55 by lglover           #+#    #+#             */
/*   Updated: 2019/04/29 16:41:49 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	map_init(int fd, t_map *map)
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
	while (i < map->rows)
		map->nodes[i++] = (t_node *)malloc(sizeof(t_node) * map->cols);
}

void	fill_row(t_map *map, char **data, int row)
{
		int col;

		col = 0;
		while (col < map->cols)
		{
			if (*data[col] == '1')
			{
				map->nodes[row][col].x = col * MM_SEC_SIZE;
				map->nodes[row][col].y = row * MM_SEC_SIZE;
				map->nodes[row][col++].collidable = true;
			}
			else
			{
				map->nodes[row][col].x = col * MM_SEC_SIZE;
				map->nodes[row][col].y = row * MM_SEC_SIZE;
				map->nodes[row][col++].collidable = false;
			}
		}
}

void	read_map(int fd, t_map *map)
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
		fill_row(map, data, i++);
		ft_strdel(&line);
	}
	close(fd);
	ft_strdel(&line);
}
