#include "wolf3d.h"

double	check_angle(double angle)
{
	if (angle < 0)
		angle = 360 + angle;
	else if (angle > 359)
		angle = angle - 360;
	return (angle);
}

void	init_time(t_time *time)
{
	ft_bzero(time, sizeof(*time));
}

void	update_time(t_time *time, t_app *app)
{
	time->prev = time->current;
	time->current = SDL_GetTicks();
	time->delta = (time->current - time->prev) * 0.001;
	time->frame += time->delta;
	app->player->speed = 250 * time->delta;
}