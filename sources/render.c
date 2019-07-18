#include "wolf3d.h"

void	shooting_animation(t_sdl *sdl, t_player *player, float time)
{
	if (time - player->anim_frame < player->weapon[player->cur_weapon].firerate)
	{
		if (!player->anim_is_done)
			gun_shoot(sdl, player, time - player->anim_frame);
		else
			printf("SHOOT: WAIT\n");
	}
	else
	{
		player->shooting = 0;
		player->anim_frame = 0;
		player->weapon[player->cur_weapon].fired = 0;
		printf("SHOOT: DONE\n");
	}
}

void	changing_animation(t_sdl *sdl, t_player *player, float time)
{
	if (time - player->anim_frame < 1.8)
	{
		if (!player->anim_is_done)
			gun_change(sdl, player, time - player->anim_frame);
		else
			printf("CHANGE: WAIT\n");
	}
	else
	{
		player->changing = 0;
		player->anim_frame = 0;
		printf("CHANGE: DONE\n");
	}
}

void	reloading_animation(t_sdl *sdl, t_player *player, float time)
{
	if (time - player->anim_frame < 1.8)
	{
		if (!player->anim_is_done)
			gun_reload(sdl, player, time - player->anim_frame);
		else
			printf("RELOAD: WAIT\n");
	}
	else
	{
		player->reloading = 0;
		player->anim_frame = 0;
		printf("CHANGE: DONE\n");
	}
}

void	idle_gun_animation(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;

	id = player->cur_weapon;
	cur_frame = (long)(delta * 1.9);
	player->idle_frame += 0.10 + (player->head_acc / 8);
	player->head_offset = sinf(player->idle_frame) * (3.5 + player->head_acc * 2) + player->head_angle;
	if (player->idle_frame > M_PI && player->head_acc > 0 && delta - player->last_step >= 0.3)
	{
		Mix_PlayChannel(1, player->steps[rand() % 8], 0);
		player->last_step = delta;
	}
	if (player->idle_frame > M_PI * 2)
		player->idle_frame = 0;
	area.y = sdl->height - 130 - 550 + player->head_offset;
	area.w = 96 * 5;
	area.x = sdl->half_width - area.w + 244;
	area.h = 116 * 5;
	cur_frame %= 2;
	if (cur_frame == 0)
		cur_frame++;
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[cur_frame], NULL, &area);
}

void	redraw(t_app *app, float frame)
{
	SDL_Rect		screen;
	int offset;

	offset = app->inputs->right_pressed ? app->inputs->zoom : 100;
	screen.x = -offset * 0.5;
	screen.y = -offset * 0.5 + app->player->head_offset;
	screen.w = app->sdl->width + offset;
	screen.h = app->sdl->height + offset;
	SDL_UpdateTexture(app->sdl->texture, NULL, app->sdl->pixels, app->sdl->width * sizeof(Uint32));
	SDL_RenderCopy(app->sdl->renderer, app->sdl->texture, NULL, &screen);
	if (app->player->shooting)
		shooting_animation(app->sdl, app->player, frame);
	if (app->player->changing)
		changing_animation(app->sdl, app->player, frame);
	if (app->player->reloading)
		reloading_animation(app->sdl, app->player, frame);
	if (app->player->anim_is_done)
		idle_gun_animation(app->sdl, app->player, frame);
	create_hud(app->sdl, app->player);
	draw_face(app->sdl, app->player, frame);
}
