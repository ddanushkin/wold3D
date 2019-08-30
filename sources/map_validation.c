/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:01 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:26:01 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		valid_characters(char *c)
{
	if (*c == '.' && *(c + 1) == '\0')
		return (1);
	if (*c == 'W' || *c == 'I' || *c == 'D' || *c == 'P')
		if (ft_isdigit(*(c + 1)) && *(c + 2) == '\0')
			return (1);
	return (0);
}

int		count_words(char const *s, char c)
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
			return (0);
		}
		rows++;
	}
	ft_delarr(tmp_rows);
	return (1);
}
