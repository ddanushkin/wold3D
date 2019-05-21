#include "wolf3d.h"

void	ft_error(char *str)
{
	ft_putendl(str);
}

void	quit_properly(t_app *app)
{
	SDL_DestroyWindow(app->sdl->window);
	SDL_Quit();
	free(app->sdl);
	free(app->map);
	free(app->player);
}
