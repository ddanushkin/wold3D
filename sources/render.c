#include "wolf3d.h"

void	draw_veiw(t_app *app)
{
	SDL_Rect		screen;

	if (app->inputs->right_pressed && app->offset < app->inputs->zoom)
		app->offset += 35;
	if (!app->inputs->right_pressed && app->offset > 50)
		app->offset -= 45;
	screen.x = -app->offset * 0.5 * 1.76;
	screen.y = -app->offset * 0.5 + app->player->head_offset;
	screen.w = app->sdl->width + app->offset * 1.76;
	screen.h = app->sdl->height + app->offset;
	SDL_UpdateTexture(app->sdl->texture, NULL, app->sdl->pixels,
			app->sdl->width * sizeof(Uint32));
	SDL_RenderCopy(app->sdl->renderer, app->sdl->texture, NULL, &screen);
}

void	redraw(t_app *app, float frame)
{
	if (app->player->state == PL_STATE_IDLE)
	{
		animation_next_frame(&app->animations[ANIM_IDLE]);
		idle_draw(app, &app->animations[ANIM_IDLE]);
	}
	if (app->player->state == PL_STATE_CHANGE)
	{
		animation_next_frame(&app->animations[ANIM_CHANGE]);
		change_draw(app, &app->animations[ANIM_CHANGE]);
		if (animation_ended(app, &app->animations[ANIM_CHANGE]))
			app->player->changed = 0;
	}
	if (app->player->state == PL_STATE_RELOAD)
	{
		animation_next_frame(&app->animations[ANIM_RELOAD]);
		reload_draw(app, &app->animations[ANIM_RELOAD]);
		if (animation_ended(app, &app->animations[ANIM_RELOAD]))
			app->player->reloaded = 0;
	}
	if (app->player->state == PL_STATE_SHOOT)
	{
		animation_next_frame(&app->animations[ANIM_SHOOT]);
		shoot_draw(app, &app->animations[ANIM_SHOOT]);
		if (animation_ended(app, &app->animations[ANIM_SHOOT]))
			app->player->weapon[app->player->cur_weapon].fired = 0;
	}
	create_hud(app->sdl, app->player);
	draw_face(app->sdl, app->player, frame);
}
