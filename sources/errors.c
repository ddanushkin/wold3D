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
	Mix_FreeMusic(app->sfx->background);
	Mix_FreeChunk(app->sfx->door_move);
	Mix_FreeChunk(app->sfx->door_open);
	app->sfx->background = NULL;
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

void	check_for_init_errors(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL video initialization error");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		ft_error("SDL audio initialization error");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		ft_error("Open Audio error");
	if (TTF_Init() < 0)
		ft_error("TTF error");
}
