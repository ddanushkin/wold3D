#include <wolf3d.h>

void end_point(int angle, t_ipoint s, t_ipoint *e, int length)
{
	e->x = s.x + (length * cos(to_rad(angle)));
	e->y = s.y + (length * sin(to_rad(angle)));
	e->y -= 15;
}

double to_rad(int angle)
{
	return (angle * (M_PI / 180.0));
}

