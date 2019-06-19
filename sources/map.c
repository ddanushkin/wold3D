/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:26:31 by ndremora          #+#    #+#             */
/*   Updated: 2019/06/19 15:06:10 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	map->objects = (t_obj *)malloc(sizeof(t_obj) * map->rows * map->cols);
	map->obj_count = 0;
	while (i < map->rows)
		map->nodes[i++] = (t_node *)malloc(sizeof(t_node) * map->cols);
}

SDL_Surface	*load_surf(char *dir, char *name, char *add)
{
	char	file_path[50];

	ft_strcpy(file_path, "../resources/");
	ft_strcat(file_path, dir);
	ft_strcat(file_path, name);
	ft_strcat(file_path, add);
	ft_strcat(file_path, ".bmp");
	return (SDL_LoadBMP(file_path));
}

void		fill_row(t_map *map, char **data, int row, t_player *player)
{
	int		col;

	col = 0;
	while (col < map->cols)
	{
		if (*data[col] >= '1' && *data[col] <= '9')
		{
			map->nodes[row][col].texture[0] = load_surf("walls/", data[col], "_n");
			map->nodes[row][col].texture[1] = load_surf("walls/", data[col], "_s");
			map->nodes[row][col].texture[2] = load_surf("walls/", data[col], "_w");
			map->nodes[row][col].texture[3] = load_surf("walls/", data[col], "_e");
			map->nodes[row][col++].collidable = true;
		}
		else if (*data[col] >= 'A' && *data[col] <= 'F')
		{
			map->objects[map->obj_count].y = row * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			map->objects[map->obj_count].x = col * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			map->objects[map->obj_count].texture = load_surf("interior/", data[col], "");
			map->nodes[row][col++].collidable = false;
			map->obj_count++;
		}
		else if (*data[col] == 'P')
		{
			player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
			map->nodes[row][col++].collidable = false;
		}
		else
			map->nodes[row][col++].collidable = false;
	}
}

void		map_read(int fd, t_map *map, t_player *player)
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
		fill_row(map, data, i++, player);
		ft_strdel(&line);
		ft_delarr(data);
	}
	close(fd);
	ft_strdel(&line);
}
