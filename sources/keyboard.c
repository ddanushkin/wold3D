#include "wolf3d.h"

static int	is_move_input(const Uint8 *key)
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

void		keyboard_input(t_app *app, float frame)
{
	const Uint8 *key;

	key = app->inputs->keyboard;
	if (is_move_input(key) && app->player->move_acc < 3.5)
		app->player->move_acc += 0.8;
	else if (!is_move_input(key) && app->player->move_acc > 0)
		app->player->move_acc -= 1.5;
	if (app->player->move_acc > 0)
		app->player->speed = 50.0 * app->player->move_acc * app->time->delta;
	if (app->player->speed > 0)
		player_movement(app->map->nodes, key, app->player);
	if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_RIGHT])
		player_rotate(app->player, key);
	if (app->inputs->left_pressed)
		animation_start(&app->animations[ANIM_SHOOT]);
	if (key[SDL_SCANCODE_Q])
		animation_start(&app->animations[ANIM_CHANGE]);
	if (key[SDL_SCANCODE_R])
		animation_start(&app->animations[ANIM_RELOAD]);
	if (key[SDL_SCANCODE_SPACE] && frame - app->player->last_space >= 0.3)
		door_interaction(app, frame);
	if (key[SDL_SCANCODE_M] || key[SDL_SCANCODE_P])
		update_sound(key, app->sfx);
}

void		player_rotate(t_player *player, const Uint8 *state)
{
	if (state[SDL_SCANCODE_LEFT])
		(player->direction -= player->speed * 0.5) < 0 ?
			player->direction = 359 : player->direction--;
	if (state[SDL_SCANCODE_RIGHT])
		(player->direction += player->speed * 0.5) > 359 ?
			player->direction = 1 : player->direction++;
	player->x_v = cos(player->direction * M_PI_180);
	player->y_v = sin(player->direction * M_PI_180);
}
