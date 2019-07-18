#include "wolf3d.h"

void		on_mouse_update(t_app *app)
{
	SDL_GetRelativeMouseState(&app->inputs->x, &app->inputs->y);
	app->inputs->left_pressed = false;
	app->inputs->right_pressed = false;
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		app->inputs->left_pressed = true;
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		app->inputs->right_pressed = true;
	if (app->inputs->y)
		app->debug_angle += app->inputs->y * 6.5 * app->inputs->sensitivity * app->time->delta;
	if (app->inputs->x)
	{
		app->player->direction += app->inputs->x * app->inputs->sensitivity * app->time->delta;
		if (app->player->direction > 359)
			app->player->direction = 0;
		if (app->player->direction < 0)
			app->player->direction = 359;
		app->player->x_v = cos(app->player->direction * M_PI_180);
		app->player->y_v = sin(app->player->direction * M_PI_180);
	}
}
