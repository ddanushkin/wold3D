#include "wolf3d.h"

static int			is_move_input(const Uint8 *key)
{
	if (key[SDL_SCANCODE_W])
		return (1);
	if (key[SDL_SCANCODE_A])
		return (1);
	if (key[SDL_SCANCODE_S])
		return (1);
	if (key[SDL_SCANCODE_D])
		return (1);
	return (0);
}

void			keyboard_input(t_app *app, float frame)
{
	const Uint8 *key;

	key = app->inputs->keyboard;
	if (is_move_input(key) && app->player->move_acc < 5)
	{
		app->player->move_acc = 2;
		//app->player->move_acc += 0.2;
	}
	else if (!is_move_input(key) && app->player->move_acc > 0)
	{
		app->player->move_acc = 0;
		//app->player->move_acc -= 0.8;
	}
	if (app->player->move_acc > 0)
		app->player->speed = 50.0 * app->player->move_acc * app->time->delta;
	if (app->player->speed > 0)
		player_movement(app->map, key, app->player);
	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(app->player, key);
	if (app->inputs->left_pressed && frame - app->player->last_shift >= 2.5)
		player_shoot(app->player, frame);
	if (key[SDL_SCANCODE_Q] && app->player->anim_frame == 0)
		player_change_weapon(app->player, frame);
	if (key[SDL_SCANCODE_R] && app->player->anim_frame == 0)
		player_reloading(app->player, frame);
	if (key[SDL_SCANCODE_SPACE] && frame - app->player->last_space >= 0.3)
		door_interaction(app, frame);
	if (key[SDL_SCANCODE_M] || key[SDL_SCANCODE_P])
		update_sound(key, app->sfx);
	if (key[SDL_SCANCODE_BACKSPACE])
		SDL_SetRelativeMouseMode(!SDL_GetRelativeMouseMode());
}

void		player_rotate(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		(player->direction -= player->speed * 0.5) < 0 ? player->direction = 359 : 0;
	if (state[SDL_SCANCODE_RIGHT])
		(player->direction += player->speed * 0.5) > 359 ? player->direction = 1 : 0;
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT])
	{
		player->x_v = cos(player->direction * M_PI_180);
		player->y_v = sin(player->direction * M_PI_180);
	}
}
