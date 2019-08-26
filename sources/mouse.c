/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 11:26:07 by lglover           #+#    #+#             */
/*   Updated: 2019/08/26 11:26:07 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		on_mouse_update(t_app *app)
{
	float	sensitivity;

	sensitivity = app->inputs->sensitivity * app->time->delta;
	SDL_GetRelativeMouseState(&app->inputs->x, &app->inputs->y);
	app->inputs->left_pressed = false;
	app->inputs->right_pressed = false;
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		app->inputs->left_pressed = true;
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		app->inputs->right_pressed = true;
	if (app->inputs->y)
	{
		app->camera_angle += app->inputs->y * 6.5 * sensitivity;
		if (app->camera_angle > app->max_angle)
			app->camera_angle = app->max_angle;
		if (app->camera_angle < -app->max_angle)
			app->camera_angle = -app->max_angle;
	}
	if (app->inputs->x)
	{
		app->player->direction += app->inputs->x * sensitivity;
		fix_direction(app);
		app->player->x_v = cos(app->player->direction * M_PI_180);
		app->player->y_v = sin(app->player->direction * M_PI_180);
	}
}
