#include "wolf3d.h"

void		update(t_app *app)
{
	const Uint8	*button;

	while (1)
	{
		SDL_UpdateTexture(app->sdl->texture, NULL, app->sdl->pixels, app->sdl->width * sizeof(Uint32));
		if (SDL_PollEvent(&app->sdl->event) && app->sdl->event.type == SDL_QUIT)
			break;
		button = SDL_GetKeyboardState(NULL);
		if (button[SDL_SCANCODE_ESCAPE])
			break;
		cast_rays(app);
		player_move(app->map, button, app->player);
		SDL_RenderCopy(app->sdl->renderer, app->sdl->texture, NULL, NULL);
		SDL_RenderPresent(app->sdl->renderer);
	}
}

int		main(void)
{
	t_app	app;
	int		fd;

	init(&app);
	player_init(app.player);
	if ((fd = open("../levels/1.wolf3d", O_RDONLY)) != -1)
	{
		map_read(fd, app.map, app.player);
		update(&app);
	}
	else
		ft_error("Map path error.");
	quit_properly(&app);
	return (0);
}
