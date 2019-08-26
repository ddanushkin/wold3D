#include "wolf3d.h"

SDL_Surface		*load_surface(char *folder, char *name)
{
	char	file_path[50];

	ft_strcpy(file_path, folder);
	ft_strcat(file_path, name);
	return (SDL_LoadBMP(file_path));
}

SDL_Texture		*load_texture(SDL_Renderer *renderer, char *folder, char *name)
{
	char			file_path[50];
	Uint32			key;
	SDL_Surface		*surface;
	SDL_Texture		*texture;

	ft_strcpy(file_path, folder);
	ft_strcat(file_path, name);
	surface = SDL_LoadBMP(file_path);
	key = SDL_MapRGB(surface->format, 152, 0, 136);
	SDL_SetColorKey(surface, SDL_TRUE, key);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}

void			load_textures(t_sdl *sdl, SDL_Texture *array[], char *path)
{
	DIR				*d;
	struct dirent	*dir;
	u_int			i;

	i = 0;
	printf("Load from: %s\n", path);
	d = opendir(path);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_name[0] == '.')
				continue ;
			else
			{
				array[i++] = load_texture(sdl->renderer, path, dir->d_name);
				printf("Sprite loaded: %s\n", dir->d_name);
			}
		}
		closedir(d);
	}
}

void			load_surfaces(SDL_Surface *array[], char *path)
{
	DIR				*d;
	struct dirent	*dir;
	u_int			i;

	i = 0;
	printf("Load from: %s\n", path);
	d = opendir(path);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (dir->d_name[0] == '.')
				continue ;
			else
			{
				array[i++] = load_surface(path, dir->d_name);
				printf("Surface loaded: %s\n", dir->d_name);
			}
		}
		closedir(d);
	}
}
