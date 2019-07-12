#include "wolf3d.h"

void	gun_shoot(t_sdl *sdl, t_player *player, float delta)
{
	SDL_Rect		area;
	int				cur_frame;
	unsigned char	id;

	cur_frame = (int)(delta * 14.0) % 11 + 1;
	id = player->cur_weapon;
	area.y = sdl->height - 130 - 550;
	area.w = 96 * 5;
	area.x = sdl->half_width - area.w + 200;
	area.h = 116 * 5;
	printf("SHOOT: FRAME %d\n", cur_frame);
	if (cur_frame == 3 && !player->weapon[id].fired)
	{
		player->weapon[id].fired++;
		player->weapon[id].ammo_cur--;
	}
	else if (cur_frame > 10)
	{
		player->anim_is_done = 1;
		SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[1],
					   NULL, &area);
		return ;
	}
	SDL_RenderCopy(sdl->renderer, player->weapon[id].sprites[cur_frame],
				   NULL, &area);
}

void	player_shoot(t_player *player, float frame)
{
	if (player->weapon[player->cur_weapon].ammo_cur > 0 && player->anim_frame == 0)
	{
		printf("SHOOT: START\n");
		player->anim_frame = frame;
		player->anim_is_done = 0;
		player->shooting = 1;
		Mix_PlayChannel(-1, player->weapon[player->cur_weapon].gun_sound, 0);
	}
	else if (player->anim_frame == 0 && player->weapon[player->cur_weapon].ammo_cur == 0)
	{
		Mix_PlayChannel(-1, player->fx_empty, 0);
		player->last_shift = frame;
	}
}

void	player_change_weapon(t_player *player, float frame)
{
	printf("CHANGE: START\n");
	player->anim_frame = frame;
	player->anim_is_done = 0;
	player->changing = 1;
}

void	player_reloading(t_player *player, float frame)
{
	unsigned char	id;

	id = player->cur_weapon;
	if (player->weapon[id].ammo_cur < player->weapon[id].ammo_max && player->weapon[id].mag_cur > 0)
	{
		printf("RELOAD: START\n");
		player->anim_frame = frame;
		player->anim_is_done = 0;
		player->reloading = 1;
		Mix_PlayChannel(-1, player->fx_reload, 0);
	}
}