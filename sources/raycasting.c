#include "wolf3d.h"

t_node		*get_node(t_map *map, float x, float y)
{
	int ix;
	int iy;

	iy = (int)y / 64;
	ix = (int)x / 64;
	return (&map->nodes[iy][ix]);
}

t_ray	*init_horz(t_player *player, double angle)
{
	t_ray	*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	ray->dist = 999999;
	ray->type = RAY_TYPE_HORZ;
	if (sin(angle) < 0)
	{
		ray->start.y = (player->y & 0xffc0) - 0.001;
		ray->step.y = -64;
	}
	else if (sin(angle) > 0)
	{
		ray->start.y = (player->y & 0xffc0) + 64.001;
		ray->step.y = 64;
	}
	else
		return (ray);
	ray->step.x = -ray->step.y / tan(-angle);
	ray->start.x = player->x + (ray->start.y - player->y) / tan(angle);
	return (ray);
}

t_ray	*init_vert(t_player *player, double angle)
{
	t_ray	*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	ray->dist = 999999;
	ray->type = RAY_TYPE_VERT;
	if (cos(angle) > 0)
	{
		ray->start.x = (player->x & 0xffc0) + 64.001;
		ray->step.x = 64;
	}
	else if (cos(angle) < 0)
	{
		ray->start.x = (player->x & 0xffc0) - 0.001;
		ray->step.x = -64;
	}
	else
		return (ray);
	ray->step.y = -ray->step.x * tan(-angle);
	ray->start.y = player->y + (ray->start.x - player->x) * tan(angle);
	return (ray);
}

int		is_wall(t_app *app, t_ray *ray)
{
	t_node *node;

	if (ray->start.x < 0 || ray->start.y < 0)
		return (1);
	if (ray->start.x >= app->map->cols * 64 || ray->start.y >= app->map->rows * 64)
		return (1);
	node = get_node(app->map, ray->start.x, ray->start.y);
	if (node->type == MAP_TYPE_EMPTY)
		return (0);
	if (node->type == MAP_TYPE_DOOR)
	{
		ray->start.x += ray->step.x * 0.5;
		ray->start.y += ray->step.y * 0.5;
		if (ray->type == RAY_TYPE_HORZ && (int)(ray->start.x) % TEXTURE_SIZE < node->door_frame)
		{
			ray->start.x -= ray->step.x * 0.5;
			ray->start.y -= ray->step.y * 0.5;
			return (0);
		}
		if (ray->type != RAY_TYPE_HORZ && (int)(ray->start.y) % TEXTURE_SIZE < node->door_frame)
		{
			ray->start.x -= ray->step.x * 0.5;
			ray->start.y -= ray->step.y * 0.5;
			return (0);
		}
		ray->node = node;
		return (1);
	}
	if (node->type == MAP_TYPE_WALL)
	{
		ray->node = node;
		return (1);
	}
	return (0);
}

void	calc_ray_data(t_app *app, t_ray *ray, float angle)
{
	if (ray->node->type == MAP_TYPE_DOOR)
	{
		ray->texture = ray->node->texture[0];
		if (ray->type == RAY_TYPE_HORZ)
			ray->offset = (int)(ray->start.x) % TEXTURE_SIZE;
		else
			ray->offset = (int)(ray->start.y) % TEXTURE_SIZE;
		ray->offset -= ray->node->door_frame;
		ray->dist = (ray->type == RAY_TYPE_HORZ) ?
					fabs((ray->start.y - app->player->y) / sin(angle)):
					fabs((ray->start.x - app->player->x) / cos(angle));
		ray->dist *= cos(angle - app->player->direction * M_PI_180);
		return;
	}
	ray->offset = (ray->type == RAY_TYPE_HORZ) ?
				  (int)ray->start.x % 64 :
				  (int)ray->start.y % 64;
	if (ray->type == RAY_TYPE_HORZ)
		ray->texture = ray->node->texture[(sin(angle) > 0 ? 0 : 1)];
	else
		ray->texture = ray->node->texture[(cos(angle) < 0 ? 3 : 2)];
	ray->dist *= cos(angle - app->player->direction * M_PI_180);
}

t_ray	*choose_ray(t_app *app, t_ray *horz, t_ray *vert, double angle)
{
	while (!is_wall(app, horz))
	{
		horz->start.x += horz->step.x;
		horz->start.y += horz->step.y;
	}
	while (!is_wall(app, vert))
	{
		vert->start.x += vert->step.x;
		vert->start.y += vert->step.y;
	}
	horz->dist = fabs((app->player->y - horz->start.y) / sin(angle));
	vert->dist = fabs((app->player->x - vert->start.x) / cos(angle));
	if (horz->dist <= vert->dist)
	{
		free(vert);
		return (horz);
	}
	else
	{
		free(horz);
		return (vert);
	}
}

t_ray	*get_ray(t_app *app, double angle)
{
	t_ray	*vert;
	t_ray	*horz;
	t_ray	*result;

	angle = angle * M_PI_180;
	horz = init_horz(app->player, angle);
	vert = init_vert(app->player, angle);
	result = choose_ray(app, horz, vert, angle);
	calc_ray_data(app, result, angle);
	return (result);
}

void	cast_single_ray(t_app *app, int x, float angle)
{
	int		slice_height;
	t_ray	*ray;

	ray = get_ray(app, angle);
	slice_height = (int)(64 / ray->dist * app->sdl->dist_to_pp);
	draw_column(app, ray, x, slice_height, angle);
	free(ray);
}

void	create_field_of_view(t_app *app)
{
	float	angle;
	float	next_angle;
	int		x;
	double	fov;

	fov = 60;
	angle = app->player->direction - (fov / 2.0);
	next_angle = fov / app->sdl->width;
	x = 0;
	while (x < app->sdl->width)
	{
		cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
}
