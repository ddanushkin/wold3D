#include "wolf3d.h"

void		on_mouse_update(t_inputs *inputs, t_player *player, float delta)
{
	inputs->left_pressed =
			SDL_GetRelativeMouseState(&inputs->x, &inputs->y) &
			SDL_BUTTON(SDL_BUTTON_LEFT);
	if (inputs->x)
	{
		player->direction += inputs->x * inputs->sensitivity * delta;
		if (player->direction > 359)
			player->direction = 0;
		if (player->direction < 0)
			player->direction = 359;
		player->x_v = cos(player->direction * M_PI_180);
		player->y_v = sin(player->direction * M_PI_180);
	}
	if (inputs->y)
		player->head_angle += inputs->y * 0.1 * delta;
}
