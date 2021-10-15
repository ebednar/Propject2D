#pragma once
#include "scene.h"
#include "camera.h"

struct GLFWwindow;

class EditorUI
{
private:
	int	width;
	int	height;
	std::map<int, std::string>	ent_types;
public:
	void	init(GLFWwindow* window, int width, int height);
	void	start_frame();
	void	end_frame();
	void	draw(Scene* scene, int fps);
	void	choose_ent(Scene* scene, Camera* cam, float x, float y);
	void	raycast_experimental(Scene* scene, Camera* cam, float x, float y);
	void	edit_target(Scene* scene);
	void	close();
};