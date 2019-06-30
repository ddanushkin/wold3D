#include "wolf3d.h"

static	void	check_for_init_errors(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ft_error("SDL video initialization error");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		ft_error("SDL audio initialization error");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		ft_error("Open Audio error");
	if (TTF_Init() < 0)
		ft_error("TTF error");
}

void	fill_diag_dist(t_app *app)
{
	int row;

	app->diag_dist = (float *)malloc(sizeof(float) * app->sdl->height);
	row = app->sdl->height;
	while (row >= 0)
	{
		app->diag_dist[row] = app->sdl->dist_to_pp * 32.0 / (row - app->sdl->height / 2);
		row--;
	}
}


static	void	create_stuff(t_app *app)
{
	Uint32	flags;
	Uint32	format;
	int		access;

	format = SDL_PIXELFORMAT_ARGB8888;
	access = SDL_TEXTUREACCESS_STATIC;
	flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	check_for_init_errors();
	app->sdl->window = SDL_CreateWindow("SDL2", 0, 0, app->sdl->width, app->sdl->height, 0);
	app->sdl->renderer = SDL_CreateRenderer(app->sdl->window, -1, flags);
	app->sdl->texture = SDL_CreateTexture(app->sdl->renderer, format, access,
										  app->sdl->width, app->sdl->height);
	app->sdl->ui = load_texture(app->sdl, "main_ui");
	app->dist_per_x = (float *)malloc(sizeof(float) * app->sdl->width);
	fill_diag_dist(app);
}

static	void	init_sdl(t_sdl *sdl)
{
	sdl->width = 1280;
	sdl->height = 720;
	sdl->fov = 3.14159 / 3.0;
	sdl->dist_to_pp = (int) (sdl->width / (tan(sdl->fov / 2.0) * 2.0));
	sdl->draw_dist = 840;
	sdl->pixels = (Uint32 *) malloc(sizeof(Uint32) * sdl->width * sdl->height);
	sdl->dist_per_x = (float *) malloc(sizeof(float) * sdl->width);
}

void			init(t_app *app)
{
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->map = (t_map *)malloc(sizeof(t_map));
	app->player = (t_player *)malloc(sizeof(t_player));
	app->sfx = (t_sfx *)malloc(sizeof(t_sfx));
	app->textures = (t_textures *)malloc(sizeof(t_textures));
	app->time = (t_time *)malloc(sizeof(t_time));
	app->textures->floors = load_surf("floors/", "1", "");
	app->textures->ceilings = load_surf("ceilings/", "1", "");
	init_sdl(app->sdl);
	create_stuff(app);
	app->sfx->background = Mix_LoadMUS("../resources/sounds/bgm.mp3");
	app->sfx->door_open_close = Mix_LoadWAV("../resources/sounds/door_open_close.wav");
	app->sfx->door_move = Mix_LoadWAV("../resources/sounds/door_move.wav");
	app->inputs->sensetivity = 1.5;
}
