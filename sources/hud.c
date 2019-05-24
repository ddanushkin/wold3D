#include "wolf3d.h"

SDL_Texture		*load_texture(t_sdl *sdl, char *name)
{
	char		file_path[50];
	SDL_Surface	*surface;
	SDL_Texture	*texture;

	file_path[0] = '\0';
	ft_strcat(file_path, "../resources/");
	ft_strcat(file_path, name);
	ft_strcat(file_path, ".bmp");
	surface = SDL_LoadBMP(file_path);
	texture = SDL_CreateTextureFromSurface(sdl->renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}

TTF_Font	*load_font(int size)
{
	TTF_Font *font;

	font = TTF_OpenFont("../resources/ui_font.ttf", size);
	return (font);
}

void	draw_text(t_sdl *sdl, t_ui_elem *ui_elem)
{
	SDL_Surface*	text_surface;
	SDL_Texture*	text_texture;
	SDL_Rect		text_area;
	SDL_Color 		color;
	TTF_Font		*font;

	font = load_font(ui_elem->size);
	color.r = 200;
	color.g = 77;
	color.b = 255;
	color.a = 255;
	text_surface = TTF_RenderText_Solid(font, ui_elem->text, color);
	text_texture = SDL_CreateTextureFromSurface(sdl->renderer, text_surface);
	SDL_FreeSurface(text_surface);
	text_area.x = ui_elem->x;
	text_area.y = ui_elem->y;
	text_area.w = text_surface->w;
	text_area.h = text_surface->h;
	SDL_RenderCopy(sdl->renderer, text_texture, NULL, &text_area);
	SDL_DestroyTexture(text_texture);
	TTF_CloseFont(font);
}

void 		create_hud(t_sdl *sdl, t_player *player)
{
	SDL_Rect area;
	t_ui_elem ui_elem;
	char *ui_text;

	area.y = sdl->height - sdl->height/5;
	area.x = 0;
	area.w = sdl->width;
	area.h = sdl->height/5;

	SDL_RenderCopy(sdl->renderer, sdl->ui, NULL, &area);

	ui_elem.y = sdl->height - 105;
	ui_elem.x = 90;
	ui_elem.text = player->cur_level;
	ui_elem.size = 60;
	draw_text(sdl, &ui_elem);

	ui_elem.x = 150 + 40;
	ui_text = ft_itoa(player->score);
	ui_elem.text = ui_text;
	draw_text(sdl, &ui_elem);
	free(ui_text);

	ui_elem.x = 150 + 40 + 235;
	ui_text = ft_itoa(player->lives);
	ui_elem.text = ui_text;
	draw_text(sdl, &ui_elem);
	free(ui_text);

	ui_elem.x = 150 + 40 + 235 + 260;
	ui_text = ft_itoa(player->health);
	ui_elem.text = ui_text;
	draw_text(sdl, &ui_elem);
	free(ui_text);

	ui_elem.x = 150 + 40 + 235 + 260 + 170;
	ui_text = ft_itoa(player->ammo);
	ui_elem.text = ui_text;
	draw_text(sdl, &ui_elem);
	free(ui_text);
}