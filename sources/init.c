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

void	fill_diag_dist(t_sdl *sdl)
{
	int		row;
	float	step;
	float	angle;
	float	dist_32;

	angle = 30.0 * M_PI_180;
	dist_32 = sdl->dist_to_pp * 32;
	step = (60.0 / sdl->width) * M_PI_180;
	sdl->diag_dist = (float *)malloc(sizeof(float) * sdl->height);
	row = sdl->height;
	while (row >= 0)
	{
		sdl->diag_dist[row] = dist_32 / (row - sdl->half_height);
		sdl->diag_dist[row] /= cosf(angle);
		angle -= step;
		row--;
	}
}

static	void	create_stuff(t_sdl *sdl, t_textures *textures)
{
	int			access;
	Uint32		flags;
	Uint32		format;

	format = SDL_PIXELFORMAT_RGB888;
	access = SDL_TEXTUREACCESS_STATIC;
	flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	check_for_init_errors();
	sdl->window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, sdl->width, sdl->height, 0);
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, flags);
	sdl->texture = SDL_CreateTexture(sdl->renderer, format, access,
			sdl->width, sdl->height);
	sdl->ui = load_texture(sdl->renderer, "../resources/", "main_ui.bmp");
	sdl->font = load_font(60);
	load_surfaces(textures->floors, "../resources/floors/");
	load_surfaces(textures->ceilings, "../resources/ceilings/");
	load_surfaces(textures->walls, "../resources/walls/");
	load_surfaces(textures->doors, "../resources/doors/");
	load_surfaces(textures->sprites, "../resources/interior/");
	fill_diag_dist(sdl);
}

static	void	init_sdl(t_sdl *sdl)
{
	float	fov;

	fov = M_PI / 6.0;
	sdl->width = 1280;
	sdl->half_width = (int)(sdl->width * 0.5);
	sdl->height = 720;
	sdl->half_height = (int)(sdl->height * 0.5);
	sdl->dist_to_pp = (int)((float)sdl->half_width / tanf(fov));
	sdl->draw_dist = 840;
	sdl->pixels = (Uint32 *)malloc(sizeof(Uint32) * sdl->width * sdl->height);
	sdl->dist_per_x = (float *)malloc(sizeof(float) * sdl->width);
}

void	malloc_textures(t_textures *textures)
{
	int files;

	files = count_files("../resources/doors/");
	textures->doors = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * files);
	files = count_files("../resources/walls/");
	textures->walls = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * files);
	files = count_files("../resources/ceilings/");
	textures->ceilings = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * files);
	files = count_files("../resources/floors/");
	textures->floors = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * files);
	files = count_files("../resources/interior/");
	textures->sprites = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * files);
}

static	void	malloc_stuff(t_app *app)
{
	app->inputs = (t_inputs *)malloc(sizeof(t_inputs));
	app->sdl = (t_sdl *)malloc(sizeof(t_sdl));
	app->map = (t_map *)malloc(sizeof(t_map));
	app->player = (t_player *)malloc(sizeof(t_player));
	app->sfx = (t_sfx *)malloc(sizeof(t_sfx));
	app->time = (t_time *)malloc(sizeof(t_time));
	app->animations = (t_animation *)malloc(sizeof(t_animation) * 10);
	app->textures = (t_textures *)malloc(sizeof(t_textures));
	malloc_textures(app->textures);
}

void			init(t_app *app)
{
	malloc_stuff(app);
	init_sdl(app->sdl);
	create_stuff(app->sdl, app->textures);
	app->sfx->background = Mix_LoadMUS("../resources/sounds/bgm.mp3");
	app->sfx->door_open = Mix_LoadWAV("../resources/sounds/door_open.wav");
	app->sfx->door_move = Mix_LoadWAV("../resources/sounds/door_move.wav");
	app->inputs->sensitivity = 1.5;
	app->inputs->zoom = 500;
	app->debug_angle = 0;
}
