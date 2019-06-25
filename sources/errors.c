#include "wolf3d.h"

void	ft_error(char *str)
{
	ft_putendl(str);
}

void	quit_properly(t_app *app)
{
	SDL_DestroyWindow(app->sdl->window);
	SDL_DestroyRenderer(app->sdl->renderer);
	SDL_DestroyTexture(app->sdl->texture);
	Mix_FreeChunk(app->player->sound_effect);
	Mix_FreeMusic(app->player->bgm);
	app->player->bgm = NULL;
	app->player->sound_effect = NULL;
	free(app->sdl->pixels);
	free(app->sdl);
	free(app->map);
	free(app->player);
	SDL_Quit();
	Mix_Quit();
	TTF_Quit();
	app = NULL;
	exit(0);
}
