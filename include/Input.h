#pragma once

struct Button
{
	bool	is_down = false;
	uint8_t	transition = 0;
};

struct Input
{
	inline static double	delta_time = 0.016;
	float	axis_x;
	float	axis_y;
	Button	button_interact;
	bool	button_up;
	bool	button_down;
	bool	button_right;
	bool	button_left;
	bool	should_exit;
};