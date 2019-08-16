#include "wolf3d.h"

void	init_shoot_anim(t_app *app, t_animation *anim)
{
	anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	anim->area->y = app->sdl->height - 130 - 550;
	anim->area->w = 96 * 5;
	anim->area->x = app->sdl->half_width - anim->area->w + 244;
	anim->area->h = 116 * 5;
	anim->play = 0;
	anim->delayed = 0;
	anim->current_frame = 0;
	anim->counter = 0;
	anim->total_frames = 11;
	anim->loop = 0;
	anim->delay = 5;
	anim->speed = 0.2f;
}

void	init_idle_anim(t_app *app, t_animation *anim)
{
	anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	anim->area->y = app->sdl->height - 130 - 550;
	anim->area->w = 96 * 5;
	anim->area->x = app->sdl->half_width - anim->area->w + 244;
	anim->area->h = 116 * 5;
	anim->play = 0;
	anim->delayed = 0;
	anim->current_frame = 0;
	anim->total_frames = 60;
	anim->loop = 1;
	anim->delay = 0;
	anim->counter = 0;
	anim->speed = 0.01f;
}

void		idle_draw(t_app *app, t_animation *anim)
{
	SDL_Texture		*sprite;

	sprite = app->player->weapon[app->player->cur_weapon].sprites[1];
	app->player->head_offset = sinf((float)app->time->current * 0.001f);
	if (app->player->move_acc > 0 && app->player->step && anim->current_frame >= 30)
	{
		Mix_PlayChannel(1, app->player->steps[rand() % 8], 0);
		app->player->step = 0;
	}
	if (anim->current_frame >= anim->total_frames - 1)
		app->player->step = 1;
	anim->area->y -= (int)(app->player->head_offset);
	SDL_RenderCopy(app->sdl->renderer, sprite, NULL, anim->area);
	anim->area->y += (int)(app->player->head_offset);
}

void		shoot_draw(t_app *app, t_animation *anim)
{
	SDL_Texture		*sprite;
	t_weapon		*weapon;

	weapon = &app->player->weapon[app->player->cur_weapon];
	if (anim->current_frame == 0)
		anim->current_frame++;
	sprite = weapon->sprites[anim->current_frame];
	if (anim->current_frame == 3 && !weapon->fired)
	{
		weapon->ammo_cur--;
		weapon->fired = 1;
	}
	SDL_RenderCopy(app->sdl->renderer, sprite, NULL, anim->area);
}