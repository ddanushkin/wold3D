#include "wolf3d.h"

char	*create_lineTEMP(int fd)
{
	char 	*read_string;
	char 	*level2;
	size_t	count;
	int 	final_length;
	int		 res;
	char 	*temp_string;

	read_string = NULL;
	level2 = NULL;
	count = 14;
	read_string = ft_strnew(count);
	//read_string = NULL;
	res = read(fd, read_string, count);
	final_length = res;
	while (1)
	{
		if (read_string[final_length - 1] != '\0')
		{
			temp_string = ft_strdup(read_string);
			res += read(fd, read_string, count);
			ft_strcat(temp_string, read_string);
			final_length += res;
		}
		else
			break;
	}
	return (level2);
}