#include "wolf3d.h"

t_node		*get_node(t_node **nodes, float x, float y)
{
	int ix;
	int iy;

	iy = (int)y / 64;
	ix = (int)x / 64;
	return (&nodes[iy][ix]);
}

t_ray		*init_horz(int x, int y, float angle)
{
	t_ray	*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	ray->dist = 999999;
	ray->type = RAY_TYPE_HORZ;
	if (sinf(angle) < 0)
	{
		ray->start.y = (y & 0xffc0) - 0.001;
		ray->step.y = -64;
	}
	else if (sinf(angle) > 0)
	{
		ray->start.y = (y & 0xffc0) + 64.001;
		ray->step.y = 64;
	}
	else
		return (ray);
	ray->step.x = -ray->step.y / tanf(-angle);
	ray->start.x = x + (ray->start.y - y) / tanf(angle);
	return (ray);
}

t_ray	*init_vert(int x, int y, float angle)
{
	t_ray	*ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	ray->dist = 999999;
	ray->type = RAY_TYPE_VERT;
	if (cosf(angle) > 0)
	{
		ray->start.x = (x & 0xffc0) + 64.001;
		ray->step.x = 64;
	}
	else if (cosf(angle) < 0)
	{
		ray->start.x = (x & 0xffc0) - 0.001;
		ray->step.x = -64;
	}
	else
		return (ray);
	ray->step.y = -ray->step.x * tanf(-angle);
	ray->start.y = y + (ray->start.x - x) * tanf(angle);
	return (ray);
}

int		is_wall(t_map *map, t_ray *ray)
{
	t_node *node;

	if (ray->start.x < 0 || ray->start.y < 0)
		return (1);
	if (ray->start.x >= map->cols * 64 || ray->start.y >= map->rows * 64)
		return (1);
	node = get_node(map->nodes, ray->start.x, ray->start.y);
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
		if (ray->type == RAY_TYPE_VERT && (int)(ray->start.y) % TEXTURE_SIZE < node->door_frame)
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

void	calc_ray_data(t_player *player, t_ray *ray, float angle)
{
	if (ray->node->type == MAP_TYPE_DOOR)
	{
		ray->texture = ray->node->texture[0];
		if (ray->type == RAY_TYPE_HORZ)
		{
			ray->offset = (int)(ray->start.x) % TEXTURE_SIZE;
			ray->dist = fabsf((ray->start.y - player->y) / sinf(angle));
		}
		else
		{
			ray->offset = (int)(ray->start.y) % TEXTURE_SIZE;
			ray->dist = fabsf((ray->start.x - player->x) / cosf(angle));
		}
		ray->offset -= ray->node->door_frame;
		ray->dist *= cos(angle - player->direction * M_PI_180);
		return ;
	}
	if (ray->type == RAY_TYPE_HORZ)
	{
		ray->offset = (int)ray->start.x % 64;
		ray->texture = ray->node->texture[(sinf(angle) > 0 ? 0 : 1)];
	}
	else
	{
		ray->offset = (int)ray->start.y % 64;
		ray->texture = ray->node->texture[(cosf(angle) < 0 ? 3 : 2)];
	}
	ray->dist *= cos(angle - player->direction * M_PI_180);
}

t_ray	*choose_ray(t_app *app, t_ray *horz, t_ray *vert, float angle)
{
	while (!is_wall(app->map, horz))
	{
		horz->start.x += horz->step.x;
		horz->start.y += horz->step.y;
	}
	while (!is_wall(app->map, vert))
	{
		vert->start.x += vert->step.x;
		vert->start.y += vert->step.y;
	}
	horz->dist = fabsf((app->player->y - horz->start.y) / sinf(angle));
	vert->dist = fabsf((app->player->x - vert->start.x) / cosf(angle));
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

t_ray	*get_ray(t_app *app, int x, float angle)
{
	t_ray	*vert;
	t_ray	*horz;
	t_ray	*result;

	angle = angle * M_PI_180;
	horz = init_horz(app->player->x, app->player->y, angle);
	vert = init_vert(app->player->x, app->player->y, angle);
	result = choose_ray(app, horz, vert, angle);
	calc_ray_data(app->player, result, angle);
	app->sdl->dist_per_x[x] = result->dist;
	return (result);
}

void	cast_single_ray(t_app *app, int x, float angle)
{
	t_ray	*ray;

	ray = get_ray(app, x, angle);
	ray->height = 64.0 / ray->dist * app->sdl->dist_to_pp;
	draw_column(app, ray, x, angle);
	free(ray);
}

void	create_field_of_view(t_app *app)
{
	float	angle;
	float	next_angle;
	int		x;

	angle = app->player->direction - (float)(60 * 0.5);
	next_angle = 60.0 / app->sdl->width;
	x = 0;
	while (x < app->sdl->width)
	{
		cast_single_ray(app, x, angle);
		angle = angle + next_angle;
		x++;
	}
}
