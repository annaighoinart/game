#include "game.h"

void	unpause(t_pr *g)
{
	g->pauseswitch = 0;
	g->bullet_count = 0;
	drawback(g);
	drawland(g);
	drawui(g);
	drawguy(g);
	bullet_fire(g);
}

void		drawui(t_pr *g)
{
	mlx_string_put(g->mlx_ptr, g->win_ptr, 5, 5, g->pointscolor, "Points: ");
	mlx_string_put(g->mlx_ptr, g->win_ptr, 76, 5, g->pointscolor, ft_itoa((int)g->points));
	mlx_string_put(g->mlx_ptr, g->win_ptr, 400, 5, g->pointscolor, "Level: ");
	mlx_string_put(g->mlx_ptr, g->win_ptr, 465, 5, g->pointscolor, ft_itoa(g->lev));
}

void	init_bullet(t_bullet *bullet, t_pr *g)
{
	const int speed[] = {5, 8, 13, 19, 25, 35, 45, 55};

	bullet->speed = speed[g->lev - 1] + rand() % 10;
	bullet->x = rand() % g->w_width;
	bullet->y = rand() % 30;
}

void	game_over(t_pr *g) {
	g->gameoverswitch = 1;
	if (g->points == 2000)
		mlx_put_image_to_window(g->mlx_ptr, g->win_ptr, g->img_win, 0, -60);
	else
		mlx_put_image_to_window(g->mlx_ptr, g->win_ptr, g->img_gameover, 0, 0);
}

int		bullet_kills(t_pr *g, t_bullet *bullet) {
	return (bullet->y >= 535 && bullet->y <= 640 - 30 && ((bullet->x >= g->xguy - 4 && bullet->x <= g->xguy + 38)
		|| (g->menubar == 2 && bullet->x >= g->xteammate - 4 && bullet->x <= g->xteammate + 38)));
}

void	bullet_fire(t_pr *g)
{
	int vero[] = {6, 5, 4, 3};
	if (!(rand() % (vero[g->lev - 1] * 15)) || !((int)g->points % 500))
	{
		if (g->bullet_count != MAX_BULLETS) {
			init_bullet(&g->bullets[g->bullet_count], g);
			g->bullet_count++;
		}
	}
	for (int i = 0; i < g->bullet_count; i++)
	{
		g->bullets[i].y += g->bullets[i].speed;
		if (g->bullets[i].y >= g->w_height) {
			init_bullet(&g->bullets[i], g);
		}
		if (bullet_kills(g, &g->bullets[i])) {
			game_over(g);
		}
		drawdrop(g, g->bullets[i].x, g->bullets[i].y);
	}
}

void		move_left(int i, t_pr *g)
{
	(g->menubar == 1 || g->menubar == 3 || i == 0) ? g->img_guy = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guy2.xpm", &g->w, &g->w) : 0;
	(g->menubar == 2 && i == 123) ? (g->img_teammate = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guy2.xpm", &g->w, &g->w)) : 0;
	if (g->lev >= 3)
	{
		(g->menubar == 1 || i == 0 || g->menubar == 3) ? g->img_guy = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guyspace2.xpm", &g->w, &g->w) : 0;
		(g->menubar == 2 && i == 123) ? (g->img_teammate = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guyspace2.xpm", &g->w, &g->w)) : 0;
	}
	if (g->xguy > 0 && (i == 0 || g->menubar == 1 || g->menubar == 3))
		g->xguy -= 8;
	else if (g->menubar == 2 && g->xteammate > 0 && i == 123)
		g->xteammate -= 8;
}

void		move_right(int i, t_pr *g)
{
	(g->menubar == 1 || i == 2 || g->menubar == 3) ? g->img_guy = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guy.xpm", &g->w, &g->w) : 0;
	(g->menubar == 2 && i == 124) ? (g->img_teammate = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guy.xpm", &g->w, &g->w)) : 0;
	if (g->lev >= 3)
	{
		(g->menubar == 1 || i == 2 || g->menubar == 3) ? g->img_guy = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guyspace.xpm", &g->w, &g->w) : 0;
		(g->menubar == 2 && i == 124) ? (g->img_teammate = mlx_xpm_file_to_image(g->mlx_ptr, "textures/guyspace.xpm", &g->w, &g->w)) : 0;
	}
	if (g->xguy < g->w_width - 64 && (i == 2 || g->menubar == 1 || g->menubar == 3))
		g->xguy += 8;
	else if (g->menubar == 2 && g->xteammate < g->w_width - 64 && i == 124)
		g->xteammate += 8;
}

void		move(int i, t_pr *g)
{
	mlx_clear_window(g->mlx_ptr, g->win_ptr);
	if ((i == 123 || i == 0) && g->pauseswitch == 0)
		move_left(i, g);
	else if ((i == 124 || i == 2) && g->pauseswitch == 0)
		move_right(i, g);
	if (g->pauseswitch == 0)
		g->points += 20;
	if (g->points == 500)
		load_second_level(g);
	else if (g->points == 1000 && g->pauseswitch == 0)
		load_third_level(g);
	else if (g->points == 1500 && g->pauseswitch == 0)
		load_fourth_level(g);	
	if (g->pauseswitch == 0 && g->gameoverswitch == 0)
	{
		drawback(g);
		drawland(g);
		drawui(g);
		drawguy(g);
		bullet_fire(g);
	}
	if (g->points == 2000 && g->pauseswitch == 0 && (g->menubar == 1 || g->menubar == 2))
		game_over(g);
	mlx_loop(g->mlx_ptr);
}
