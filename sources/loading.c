#include "wolf3d.h"

SDL_Texture		*load_texture(SDL_Renderer *renderer, char *name)
{
	char		file_path[50];
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	Uint32		key;

	ft_strcpy(file_path, "../resources/");
	ft_strcat(file_path, name);
	ft_strcat(file_path, ".bmp");
	surface = SDL_LoadBMP(file_path);
	key = SDL_MapRGB(surface->format, 152, 0, 136);
	SDL_SetColorKey(surface, SDL_TRUE, key);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}

SDL_Texture		*load_sprite(SDL_Renderer *renderer, char *folder_path, char *sprite_name)
{
	char			file_path[50];
	Uint32			key;
	SDL_Surface		*surface;
	SDL_Texture		*texture;

	ft_strcpy(file_path, folder_path);
	ft_strcat(file_path, sprite_name);
	surface = SDL_LoadBMP(file_path);
	key = SDL_MapRGB(surface->format, 152, 0, 136);
	SDL_SetColorKey(surface, SDL_TRUE, key);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}
