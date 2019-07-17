#include "wolf3d.h"

void	init_time(t_app *app, t_fps *fps)
{
	ft_bzero(app->time, sizeof(*app->time));
	ft_bzero(fps, sizeof(*fps));
	fps->lasttime = SDL_GetTicks();
}

void	update_time(t_app *app)
{
	app->time->prev = app->time->current;
	app->time->current = SDL_GetTicks();
	app->time->delta = (app->time->current - app->time->prev) * 0.001;
	app->time->frame += app->time->delta;
}

void	get_fps(t_fps *fps, SDL_Renderer *renderer)
{
	fps->frames++;
	if (fps->lasttime < SDL_GetTicks() - FPS_INTERVAL * 1000)
	{
		fps->lasttime = SDL_GetTicks();
		fps->current = fps->frames;
		fps->frames = 0;
	}
	debug_show_fsp(renderer, fps->current);
}

int		check_for_quit(t_app *app)
{
	if (app->sdl->event.type == SDL_QUIT)
		return (1);
	if (app->inputs->keyboard[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}