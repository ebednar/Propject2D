#pragma once

struct Events
{
	float	last_x;
	float	last_y;
	float	yaw;
	float	pitch;
	float	xoffset;
	float	yoffset;
	int		width;
	int		height;
	bool	keys[1024];
	bool	first_m = true;
	bool	l_clicked = false;
	bool	r_clicked = false;
	bool	resize;
};

void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void	mouse_callback(GLFWwindow *window, double xpos, double ypos);
void	mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void	mouse_scrolling(Events controls);
void	window_size_callback(GLFWwindow* window, int width, int height);