/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 15:56:55 by lglover           #+#    #+#             */
/*   Updated: 2019/04/29 16:14:21 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	map_init(int fd, t_map *map)
{
	char *line;
	char **data;

	line = NULL;
	ft_gnl(fd, &line);
	data = ft_strsplit(line, ' ');
	map->rows = ft_atoi(data[0]);
	map->cols = ft_atoi(data[1]);
	ft_delarr(data);
	map->nodes = (t_node **)malloc(sizeof(t_node *) * map->rows);
}

void	read_map(int fd, t_map *map)
{
	char	*line;
	char	**data;
	int		res;

	line = NULL;
	map_init(fd, map);
	while ((res = ft_gnl(fd, &line)) > 0)
	{
		ft_strdel(&line);
	}
	close(fd);
	ft_strdel(&line);
}
