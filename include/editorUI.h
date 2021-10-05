#pragma once
#include "scene.h"

struct GLFWwindow;

class EditorUI
{
private:
	
public:
	void	init(GLFWwindow* window);
	void	draw(Scene* scene, int width, int height, int fps);
	void	close();
};