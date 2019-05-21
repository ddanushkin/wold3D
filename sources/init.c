#include "wolf3d.h"

void		init(t_app *app)
{
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->map = (t_map *)malloc(sizeof(t_map));
	app->player = (t_player *)malloc(sizeof(t_player));
	app->sdl->width = 640;
	app->sdl->height = 480;
	app->sdl->fov = 3.14159 / 3.0; //0.66;
	app->sdl->dist_to_pp = app->sdl->width / (tan(app->sdl->fov / 2.0) * 2.0);
	app->sdl->draw_dist = 840;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL initialization error");
	app->sdl->window = SDL_CreateWindow("SDL2 Example", 0, 0, app->sdl->width, app->sdl->height, 0);
	app->sdl->screen = SDL_GetWindowSurface(app->sdl->window);
	app->sdl->ceiling = -5;
}
