#include "wolf3d.h"

void	draw_text(t_sdl *sdl, const char *text, int x, int y)
{
	SDL_Surface*	text_surface;
	SDL_Texture*	text_texture;
	SDL_Rect		text_area;

	text_surface = TTF_RenderText_Solid(sdl->font, text, sdl->text_color);
	text_texture = SDL_CreateTextureFromSurface(sdl->renderer, text_surface);
	SDL_FreeSurface(text_surface);
	text_area.x = x;
	text_area.y = y;
	text_area.w = text_surface->w;
	text_area.h = text_surface->h;
	SDL_RenderCopy(sdl->renderer, text_texture, NULL, &text_area);
	SDL_DestroyTexture(text_texture);
}