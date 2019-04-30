#include "wolf3d.h"

void	ft_error(char *str)
{
	ft_putendl(str);
	exit(0);
}

void	quit_properly(t_app *app)
{
	SDL_DestroyRenderer(app->sdl.ren);
	SDL_DestroyWindow(app->sdl.win);
	SDL_Quit();
}
