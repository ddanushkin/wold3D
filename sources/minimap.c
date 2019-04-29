/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 16:55:16 by lglover           #+#    #+#             */
/*   Updated: 2019/04/29 17:03:50 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw_minimap(t_map *map)
{
	int		row;
	int		col;

	col = 0;
	while (col < map->cols)
	{
		row = 0;
		while (row < map->rows)
		{
			if (map->nodes[row][col].collidable)
				printf("[#]");
			else
				printf("[ ]");
			row++;
		}
		printf("\n");
		col++;
	}
}
