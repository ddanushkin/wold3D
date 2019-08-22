#include "wolf3d.h"

void	animation_start(t_animation *anim)
{
	if (!anim->play && !anim->delayed)
	{
		anim->play = 1;
		anim->counter = 0;
		anim->current_frame = 0;
	}
}

void	animations_update(t_app *app)
{
	int i = 0;
	t_animation *anim;

	while (i < ANIM_COUNT)
	{
		anim = &app->animations[i];
		if (anim->counter < anim->speed || anim->delayed)
			anim->counter += app->time->delta;
		if (anim->counter >= anim->delay)
			anim->delayed = 0;
		i++;
	}
}

void	animation_next_frame(t_animation *anim)
{
	int last_frame;

	if (!anim->play)
		return ;
	while (anim->counter >= anim->speed)
	{
		anim->counter -= anim->speed;
		anim->current_frame++;
		if (anim->loop)
			anim->current_frame %= anim->total_frames;
	}
	last_frame = (anim->current_frame >= anim->total_frames - 1);
	if (last_frame)
		anim->current_frame = anim->total_frames - 1;
	if (!anim->loop && last_frame)
	{
		anim->play = 0;
		anim->delayed = 1;
	}
}

int		animation_ended(t_app *app, t_animation *anim)
{
	if (!anim->play && anim->counter >= anim->speed)
	{
		app->player->state = PL_STATE_IDLE;
		return (1);
	}
	return (0);
}