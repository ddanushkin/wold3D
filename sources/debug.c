#include "wolf3d.h"

void	debug_player(t_app *app)
{
	const Uint8 *key;

	key = app->inputs->keyboard;
	if (key[SDL_SCANCODE_EQUALS] && app->player->health < 100)
		app->player->health += 1;
	if (key[SDL_SCANCODE_MINUS] && app->player->health > 0)
		app->player->health -= 1;
	if (key[SDL_SCANCODE_EQUALS] || key[SDL_SCANCODE_MINUS])
		printf("debug -> %d\n", app->player->health);
}

void	debug_show_fps(SDL_Renderer *renderer, int fps)
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

void	get_fps(t_fps *fps, SDL_Renderer *renderer)
{
	fps->frames++;
	if (fps->lasttime < SDL_GetTicks() - FPS_INTERVAL)
	{
		fps->lasttime = SDL_GetTicks();
		fps->current = fps->frames;
		fps->frames = 0;
	}
	debug_show_fps(renderer, fps->current);
}
