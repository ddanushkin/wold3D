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

int		load_texture(t_node *node, char *name)
{
	char file_path[50];

	file_path[0] = '\0';
	ft_strcat(file_path, "../resources/");
	ft_strcat(file_path, name);
	ft_strcat(file_path, ".bmp");
	node->texture = SDL_LoadBMP(file_path);

	if (node->texture == NULL)
	{
		printf("Failed to load right image!\n");
		return (0);
	}
	return (1);
}

void	fill_row(t_map *map, char **data, int row, t_player *player)
{
		int		col;

		col = 0;
		while (col < map->cols)
		{
			if (*data[col] >= '1' && *data[col] <= '9')
			{
				load_texture(&map->nodes[row][col], data[col]);
				map->nodes[row][col].x = col * TEXTURE_SIZE;
				map->nodes[row][col].y = row * TEXTURE_SIZE;
				map->nodes[row][col++].collidable = true;
			}
			else if (*data[col] == 'P')
			{
				player->y = row * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
				player->x = col * TEXTURE_SIZE + (TEXTURE_SIZE / 2);
				map->nodes[row][col].x = col * TEXTURE_SIZE;
				map->nodes[row][col].y = row * TEXTURE_SIZE;
				map->nodes[row][col++].collidable = false;
			}
			else
			{
				map->nodes[row][col].x = col * TEXTURE_SIZE;
				map->nodes[row][col].y = row * TEXTURE_SIZE;
				map->nodes[row][col++].collidable = false;
			}
		}
}

void	map_read(int fd, t_map *map, t_player *player)
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
