/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:52:13 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:55:43 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	map_malloc(t_map *map)
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

static void	map_fill(t_app *app)
{
	int		row;
	int		col;

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

void		map_init(t_app *app)
{
	map_malloc(app->map);
	map_fill(app);
}
