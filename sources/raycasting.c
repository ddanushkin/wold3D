#include "wolf3d.h"

void	draw_ray_collision(t_sdl *sdl, t_player *player, int x, int y, int r, int g, int b)
{
	SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(sdl->renderer, player->x, player->y, x, y);
	SDL_SetRenderDrawColor(sdl->renderer, r, g, b, 255);
	SDL_RenderDrawPoint(sdl->renderer, x, y-1);
	SDL_RenderDrawPoint(sdl->renderer, x-1, y);
	SDL_RenderDrawPoint(sdl->renderer, x, y+1);
	SDL_RenderDrawPoint(sdl->renderer, x+1, y);
	SDL_RenderDrawPoint(sdl->renderer, x+1, y-1);
	SDL_RenderDrawPoint(sdl->renderer, x-1, y+1);
	SDL_RenderDrawPoint(sdl->renderer, x+1, y+1);
	SDL_RenderDrawPoint(sdl->renderer, x-1, y-1);
}

float get_dist(int p_x, int p_y, float r_x, float r_y)
{
	float d_x;
	float d_y;

	d_x = p_x - r_x;
	d_y = p_y - r_y;
	return sqrtf(d_x * d_x + d_y * d_y);
}

int is_wall_h(t_map *map, t_fpoint *pos, t_ray *ray, double angle)
{
	int		y;
	int		x;

	y = pos->y / 64;
	x = pos->x / 64;
	if (pos->x < 0 || pos->y < 0 || pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	if (sin(angle) > 0)
		ray->texture = map->nodes[y][x].texture_n;
	else
		ray->texture = map->nodes[y][x].texture_n;
	ray->x = pos->x;
	ray->y = pos->y;
	return map->nodes[y][x].collidable;
}

int is_wall_v(t_map *map, t_fpoint *pos, t_ray *ray, double angle)
{
	int		y;
	int		x;

	y = pos->y / 64;
	x = pos->x / 64;
	if (pos->x < 0 || pos->y < 0 || pos->x >= map->cols * 64 || pos->y >= map->rows * 64)
		return (1);
	if (cos(angle) < 0)
		ray->texture = map->nodes[y][x].texture_n;
	else
		ray->texture = map->nodes[y][x].texture_n;
	ray->x = pos->x;
	ray->y = pos->y;
	return map->nodes[y][x].collidable;
}

t_ray	*cast_ray_horz(t_map *map, t_player *player, double angle)
{
	t_fpoint start;
	t_fpoint step;
	t_ray *ray;

	ray = (t_ray *)malloc(sizeof(t_ray));

	angle = angle * 3.14 / 180.0;
	if (sin(angle) < 0)
	{
		start.y = (player->y / 64) * 64 - 1;
		step.y = -64;
	}
	else if(sin(angle) > 0)
	{
		start.y = (player->y / 64) * 64 + 64;
		step.y = 64;
	}
	else
	{
		ray->dist = 99999;
		ray->offset = 0;
		ray->texture = NULL;
		return ray;
	}
	step.x = 64 / tan(angle);
	step.x = cos(angle) <= 0 ? -fabsf(step.x) : fabsf(step.x);
	start.x = player->x + (start.y - player->y) / tan(angle);

	while (!is_wall_h(map, &start, ray, angle))
	{
		start.x += step.x;
		start.y += step.y;
	}
	ray->dist = get_dist(player->x, player->y, start.x, start.y);
	ray->offset = (int)start.x % 64;
	return ray;
}

t_ray	*cast_ray_vert(t_map *map, t_player *player, double angle)
{
	t_fpoint start;
	t_fpoint step;
	t_ray *ray;

	ray = (t_ray *)malloc(sizeof(t_ray));
	angle = angle * 3.14 / 180.0;
	if (cos(angle) > 0)
	{
		start.x = (player->x / 64) * 64 + 64;
		step.x = 64;
	}
	else if(cos(angle) < 0)
	{
		start.x = (player->x / 64) * 64 - 1;
		step.x = -64;
	}
	else
	{
		ray->dist = 99999;
		ray->offset = 0;
		ray->texture = NULL;
		return ray;
	}
	step.y = 64 * tan(angle);
	step.y = sin(angle) >= 0 ? fabsf(step.y) : -fabsf(step.y);
	start.y = player->y + (start.x - player->x) * tan(angle);

	while (!is_wall_v(map, &start, ray, angle))
	{
		start.x += step.x;
		start.y += step.y;
	}
	ray->dist = get_dist(player->x, player->y, start.x, start.y);
	ray->offset = (int)start.y % 64;
	return ray;
}

void		cast_ray(t_app *app, int x, float angle)
{
	t_ray		*ray_vert;
	t_ray		*ray_horz;
	t_ray		*ray;
	int			slice_height;

	ray_horz = cast_ray_horz(app->map, app->player, angle);
	ray_vert = cast_ray_vert(app->map, app->player, angle);
	ray = ray_horz->dist < ray_vert->dist ? ray_horz : ray_vert;
	ray->dist *= cos((angle - app->player->direction) * M_PI / 180.0);
	slice_height = 64 / ray->dist * app->sdl->dist_to_pp;
	draw_col(app->sdl, ray, x, slice_height);
	free(ray_horz);
	free(ray_vert);
}

void		cast_rays(t_app *app)
{
	float		angle;
	int			x;

	angle = app->player->direction - 30.0;
	x = 0;
	while (x < app->sdl->width)
	{
		cast_ray(app, x, angle);
		angle = angle + (60.0 / app->sdl->width);
		x++;
	}
}