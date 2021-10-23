#pragma once
#include "scene.h"
#include "camera.h"

struct GLFWwindow;

class EditorUI
{
public:
	bool	is_edit_tilemap = false;
private:
	std::map<int, std::string>	ent_types;
	std::string	scene_path;
	glm::mat4	projection;
	GLFWwindow*	window;
	int		width;
	int		height;
	bool	save_enable = false;
public:
	void	init(GLFWwindow* window, int width, int height);
	void	start_frame();
	void	end_frame();
	void	draw(Scene* scene, int fps);
	bool	choose_ent(Scene* scene, Camera* cam, float x, float y);
	void	edit_tilemap(Scene* scene);
	void	raycast_experimental(Scene* scene, Camera* cam, float x, float y);
	void	edit_target(Scene* scene);
	void	recalc_proj(int width, int height);
	void	close();
};