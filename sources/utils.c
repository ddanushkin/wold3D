#include "wolf3d.h"

void	init_time(t_app *app)
{
	ft_bzero(app->time, sizeof(*app->time));
}

void	update_time(t_app *app)
{
	app->time->prev = app->time->current;
	app->time->current = SDL_GetTicks();
	app->time->delta = (app->time->current - app->time->prev) * 0.001;
	app->time->frame += app->time->delta;
}

int		check_for_quit(t_app *app)
{
	if (app->sdl->event.type == SDL_QUIT)
		return (1);
	if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}