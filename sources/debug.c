#include "wolf3d.h"

void		debug_player(t_app *app)
{
	const Uint8 *key;

	key = app->inputs->keyboard;
//	if (key[SDL_SCANCODE_EQUALS] &&
//		app->inputs->sensitivity < 5)
//		app->inputs->sensitivity += 0.1;
//	if (key[SDL_SCANCODE_MINUS] &&
//			app->inputs->sensitivity > 0)
//		app->inputs->sensitivity -= 0.1;
//	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
//	{
//		if (app->inputs->sensitivity < 0)
//			app->inputs->sensitivity = 0;
//		if (app->inputs->sensitivity > 5)
//			app->inputs->sensitivity = 5;
//		printf("sensitivity -> %f\n", app->inputs->sensitivity);
//	}

//	if (key[SDL_SCANCODE_EQUALS] &&
//		app->player->obj_dist < (64 * 5))
//		app->player->obj_dist += 5;
//	if (key[SDL_SCANCODE_MINUS] &&
//		app->player->obj_dist > 0)
//		app->player->obj_dist -= 5;
//	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
//	{
//		if (app->player->obj_dist < 0)
//			app->player->obj_dist = 0;
//		if (app->player->obj_dist > 64 * 5)
//			app->player->obj_dist = 64 * 5;
//		printf("obj_dist -> %f\n", app->player->obj_dist);
//	}
	if (key[SDL_SCANCODE_EQUALS])
		app->debug_angle++;
	if (key[SDL_SCANCODE_MINUS])
		app->debug_angle--;
	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
	{
		if (app->debug_angle < 0)
			app->debug_angle = 359;
		if (app->debug_angle > 359)
			app->debug_angle = 0;
		printf("debug -> %f\n", app->debug_angle);
	}
}

void		debug_show_fsp(SDL_Renderer *renderer, int fps)
{
	t_ui_elem	ui_elem;
	char		*ui_text;

	ui_elem.y = -10;
	ui_elem.x = 5;
	ui_elem.size = 25;
	ui_text = ft_itoa(fps);
	ui_elem.text = ui_text;
	draw_text(renderer, &ui_elem);
	free(ui_text);
}