#include "wolf3d.h"

void	init_test_animation(t_app *app, t_animation *anim)
{
    anim->start_frame = app->time->frame_nbr;
    anim->delay_frames = 200;
    anim->total_frames = 10;
    anim->play_once = 1;

    anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    anim->area->y = app->sdl->height - 130 - 550;
    anim->area->w = 96 * 5;
    anim->area->x = app->sdl->half_width - anim->area->w + 200;
    anim->area->h = 116 * 5;

    anim->current_frame = 0;
    anim->finished = 0;
    anim->played = 0;
}

void	init_idle_animation(t_app *app, t_animation *anim)
{
    anim->area = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    anim->area->y = app->sdl->height - 130 - 550;
    anim->area->w = 96 * 5;
    anim->area->x = app->sdl->half_width - anim->area->w + 244;
    anim->area->h = 116 * 5;

    anim->start_frame = app->time->frame_nbr;
    anim->total_frames = 60;
    anim->play_once = 0;
    anim->delay_frames = 0;
    anim->current_frame = 0;
    anim->finished = 0;
    anim->played = 0;
    app->player->step = 1;
}

void		idle_draw(t_app *app, t_animation *anim)
{
    SDL_Texture		*sprite;

    sprite = app->player->weapon[app->player->cur_weapon].sprites[1];
    app->player->idle_frame += 0.10 + (app->player->head_acc / 8);
    app->player->head_offset = sinf(app->player->idle_frame) * (3.5 + app->player->head_acc * 2);
    if (app->player->move_acc > 0 && app->player->step && anim->current_frame >= 30)
    {
        Mix_PlayChannel(1, app->player->steps[rand() % 8], 0);
        app->player->step = 0;
    }
    if (anim->current_frame >= anim->total_frames - 1)
        app->player->step = 1;
    anim->area->y += (int)app->player->head_offset;
    SDL_RenderCopy(app->sdl->renderer, sprite, NULL, anim->area);
    anim->area->y -= (int)app->player->head_offset;
}

void		animation_draw_sprite(t_app *app, t_animation *anim)
{
    SDL_Texture *sprite;

    sprite = app->player->weapon[0].sprites[anim->current_frame];
    SDL_RenderCopy(app->sdl->renderer, sprite, NULL, anim->area);
}

void	animation_next_frame(t_app *app, t_animation *anim)
{
    if (anim->finished)
        return ;
    if (!anim->played)
        anim->current_frame = (app->time->frame_nbr) % anim->total_frames;
    if (anim->current_frame == 0)
        anim->current_frame++;
    if (anim->play_once && anim->current_frame >= anim->total_frames - 1)
        anim->played = 1;
    if (app->time->frame_nbr - anim->start_frame >= anim->delay_frames)
        anim->finished = 1;
    if (!anim->delay_frames)
        anim->finished = 0;
}
