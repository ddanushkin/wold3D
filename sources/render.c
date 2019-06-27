#include "wolf3d.h"

void	shooting_animation(t_sdl *sdl, t_player *player, float time)
{
	if (time - player->anim_frame < player->weapon[player->cur_weapon].firerate)
	{
		if (!player->anim_is_done)
			gun_shoot(sdl, player, time - player->anim_frame);
		else
			printf("SHOOT: WAIT\n");
	}
	else
	{
		player->shooting = 0;
		player->anim_frame = 0;
		player->weapon[player->cur_weapon].fired = 0;
		printf("SHOOT: DONE\n");
	}
}

void	changing_animation(t_sdl *sdl, t_player *player, float time)
{
	if (time - player->anim_frame < 1.8)
	{
		if (!player->anim_is_done)
			gun_change(sdl, player, time - player->anim_frame);
		else
			printf("CHANGE: WAIT\n");
	}
	else
	{
		player->changing = 0;
		player->anim_frame = 0;
		printf("CHANGE: DONE\n");
	}
}

void	reloading_animation(t_sdl *sdl, t_player *player, float time)
{
	if (time - player->anim_frame < 1.8)
	{
		if (!player->anim_is_done)
			gun_reload(sdl, player, time - player->anim_frame);
		else
			printf("RELOAD: WAIT\n");
	}
	else
	{
		player->reloading = 0;
		player->anim_frame = 0;
		printf("CHANGE: DONE\n");
	}
}

void	idle_gun_animation(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;

	id = player->cur_weapon;
	cur_frame = (long)(delta * 1.9);

	player->idle_frame += 0.10 + (player->acc / 8);
	player->head_offset = sinf(player->idle_frame) * (3.5 + player->acc * 2);
	if (player->idle_frame > M_PI * 2)
		player->idle_frame = 0;
	area.y = sdl->height - 130 - 550 + player->head_offset;

	area.w = 96 * 5;
	area.x = sdl->width / 2 - area.w + 200;
	area.h = 116 * 5;
	cur_frame %= 2;
	if (cur_frame == 0)
		cur_frame++;
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[cur_frame], NULL, &area);
}

void	redraw(t_sdl *sdl, t_player *player, t_time *time)
{
	SDL_UpdateTexture(sdl->texture, NULL, sdl->pixels, sdl->width * sizeof(Uint32));
	SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
	if (player->shooting)
		shooting_animation(sdl, player, time->frame);
	if (player->changing)
		changing_animation(sdl, player, time->frame);
	if (player->reloading)
		reloading_animation(sdl, player, time->frame);
	if (player->anim_is_done)
		idle_gun_animation(sdl, player, time->frame);
	create_hud(sdl, player);
	draw_face(sdl, player, time->frame);
	SDL_RenderPresent(sdl->renderer);
}
