#include "wolf3d.h"

void	draw_slice(t_app *app, t_node *obj, t_iiter x, t_iiter y)
{
	int			texture_x;
	int			texture_y;
	int			corrected_y;
	t_color		col;

	corrected_y = y.cur + app->player->head_offset;
	if (corrected_y < 0 || corrected_y >= app->sdl->height)
		return ;
	texture_x = (x.cur - x.min) * (64.0 / obj->height);
	texture_y = (y.cur - y.min) * (64.0 / obj->height);
	get_color(obj->texture[0], &col, texture_x, texture_y);
	if (!(col.r == 152 && col.g == 0 && col.b == 136))
	{
		shade_color(obj->dist + 150, &col, app->sdl->draw_dist);
		set_pixel(app->sdl, x.cur, corrected_y, &col);
	}
}

void	draw_object(t_app *app, t_node *obj)
{
	t_iiter	x;
	t_iiter	y;

	x.min = obj->screen_x - obj->height * 0.5;
	x.max = x.min + obj->height;
	y.min = app->sdl->half_height - obj->height * 0.5;
	y.max = y.min + obj->height;
	if (x.max > app->sdl->width)
		x.max = app->sdl->width;
	if (y.max > app->sdl->height)
		y.max = app->sdl->height;
	if ((x.cur = x.min) < 0)
		x.cur = 0;
	while (x.cur < x.max)
	{
		if ((y.cur = y.min) < 0)
			y.cur = 0;
		while (app->sdl->dist_per_x[x.cur] > obj->dist && y.cur < y.max)
		{
			draw_slice(app, obj, x, y);
			y.cur++;
		}
		x.cur++;
	}
}

void	fill_object(t_sdl *sdl, t_player *player, t_node *object)
{
	int		dx;
	int		dy;
	float	angle;

	dx = object->center.x - player->x;
	dy = object->center.y - player->y;
	angle = atan2f(dy, dx) - (player->direction * M_PI_180);
	object->dist = sqrtf(dx * dx + dy * dy);
	object->screen_x = tanf(angle) * sdl->dist_to_pp + sdl->half_width;
	object->height = (int)(64 / object->dist * sdl->dist_to_pp);
}

void	update_objects(t_app *app)
{
	int		i;
	t_node	*object;

	i = 0;
	while (i < app->map->objects_count)
	{
		object = app->map->objects[i];
		if (object->visible)
		{
			fill_object(app->sdl, app->player, object);
			draw_object(app, object);
		}
		i++;
	}
}

void	reset_objects(t_map *map)
{
	int		i;
	t_node	*object;

	i = 0;
	while (i < map->objects_count)
	{
		object = map->objects[i];
		object->screen_x = -1;
		object->height = -1;
		object->dist = -1;
		i++;
	}
}
