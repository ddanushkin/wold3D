#include "wolf3d.h"

double	check_angle(double angle)
{
	if (angle < 0)
		angle = 360 + angle;
	else if (angle > 359)
		angle = angle - 360;
	return (angle);
}