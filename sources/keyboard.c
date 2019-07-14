#include "wolf3d.h"

static	void	player_shoot(t_player *player, float frame)
{
	if (player->weapon[player->cur_weapon].ammo_cur > 0 && player->anim_frame == 0)
	{
		printf("SHOOT: START\n");
		player->anim_frame = frame;
		player->anim_is_done = 0;
		player->shooting = 1;
		Mix_PlayChannel(-1, player->weapon[player->cur_weapon].gun_sound, 0);
	}
	else if (player->anim_frame == 0 && player->weapon[player->cur_weapon].ammo_cur == 0)
	{
		Mix_PlayChannel(-1, player->fx_empty, 0);
		player->last_shift = frame;
	}
}

static	void	player_change_weapon(t_player *player, float frame)
{
	printf("CHANGE: START\n");
	player->anim_frame = frame;
	player->anim_is_done = 0;
	player->changing = 1;
}

static	void	player_reloading(t_player *player, float frame)
{
	unsigned char	id;

	id = player->cur_weapon;
	if (player->weapon[id].ammo_cur < player->weapon[id].ammo_max && player->weapon[id].mag_cur > 0)
	{
		printf("RELOAD: START\n");
		player->anim_frame = frame;
		player->anim_is_done = 0;
		player->reloading = 1;
		Mix_PlayChannel(-1, player->fx_reload, 0);
	}
}

int			is_move_input(const Uint8 *key)
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
