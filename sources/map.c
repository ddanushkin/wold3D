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

int		loadMedia(t_node *node)
{
	char *s2 = node->name;
	char s1[99] = "../resources/";
	ft_strcat(s1, s2);
	node->texture = SDL_LoadBMP(s1);
	if (node->texture == NULL)
	{
		printf( "Failed to load right image!\n" );
		return (0);
	}
	return (1);
}

void	fill_row(t_map *map, char **data, int row, t_player *player)
{
		int col;

		col = 0;
		while (col < map->cols)
		{
			if (*data[col] >= '1' && *data[col] <= '5')
			{
				map->nodes[row][col].name = ft_strcat(data[col], ".bmp");
				if (loadMedia(&map->nodes[row][col]) == 0)
				{
					ft_error("ti sam znaesh");
				}

				//map->nodes[row][col].texture = SDL_LoadBMP(ft_strcat("../resources/", map->nodes[row][col].name));
				map->nodes[row][col].x = col * MM_SEC_SIZE;
				map->nodes[row][col].y = row * MM_SEC_SIZE;
				map->nodes[row][col++].collidable = true;
			}
			else if (*data[col] == 'P')
			{
				player->y = row * MM_SEC_SIZE + (MM_SEC_SIZE / 2);
				player->x = col * MM_SEC_SIZE + (MM_SEC_SIZE / 2);
				map->nodes[row][col].x = col * MM_SEC_SIZE;
				map->nodes[row][col].y = row * MM_SEC_SIZE;
				map->nodes[row][col++].collidable = false;
			}
			else
			{
				map->nodes[row][col].x = col * MM_SEC_SIZE;
				map->nodes[row][col].y = row * MM_SEC_SIZE;
				map->nodes[row][col++].collidable = false;
			}
		}
}

void	read_map(int fd, t_map *map, t_player *player)
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
