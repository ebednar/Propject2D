#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "events.h"

void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	Events* events;

	events = static_cast<Events*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		events->game_input.should_exit = true;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			events->keys[key] = true;
		else if (action == GLFW_RELEASE)
			events->keys[key] = false;
	}
}

void	mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	Events* events;
	const float	sensitivity = 0.05f;
	float xoffset;
	float yoffset;

	events = static_cast<Events*>(glfwGetWindowUserPointer(window));
	if (events->first_m)
	{
		events->last_x = xpos;
		events->last_y = ypos;
		events->first_m = false;
	}
	xoffset = xpos - events->last_x;
	yoffset = events->last_y - ypos;
	events->last_x = xpos;
	events->last_y = ypos;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	events->xoffset = xoffset;
	events->yoffset = yoffset;
	events->yaw += xoffset;
	events->pitch += yoffset;
	if (events->pitch > 89.0f)
		events->pitch = 89.0f;
	if (events->pitch < -89.0f)
		events->pitch = -89.0f;
}

void	mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Events* events;

	events = static_cast<Events*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		events->r_clicked = true;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		events->r_clicked = false;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		events->l_clicked = true;
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		events->l_clicked = false;
		events->l_released = true;
	}
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	Events* events;

	events = static_cast<Events*>(glfwGetWindowUserPointer(window));
	events->width = width;
	events->height = height;
	events->resize = true;
}