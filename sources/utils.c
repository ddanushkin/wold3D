#include "wolf3d.h"

void	init_time(t_time *time, t_fps *fps)
{
	ft_bzero(time, sizeof(*time));
	ft_bzero(fps, sizeof(*fps));
	fps->lasttime = SDL_GetTicks();
	time->current = SDL_GetTicks();
}

void	update_time(t_time *time)
{
	time->prev = time->current;
	time->current = SDL_GetTicks();
	time->delta = (time->current - time->prev) * 0.001;
	time->frame += time->delta;
	time->frame_nbr++;
	if (time->frame_nbr < 0)
		time->frame_nbr = 0;
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

int		check_for_quit(SDL_Event *event, t_inputs *inputs)
{
	if (event->type == SDL_QUIT)
		return (1);
	if (inputs->keyboard[SDL_SCANCODE_ESCAPE])
		return (1);
	return (0);
}
