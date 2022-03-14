#pragma once
#include "scene.h"
#include "camera.h"
#include "editor/content_browser.h"

struct GLFWwindow;

class EditorUI
{
public:
	bool	is_edit_tilemap = false;
private:
	ContentBrowser	browser;
	std::map<int, std::string>	ent_types;
	std::string	scene_path;
	glm::mat4	projection;
	GLFWwindow*	window;
	int		width;
	int		height;
	bool	save_enable = false;
public:
	void	init(GLFWwindow* window, int width, int height, bool scene_loaded, std::string scene_path);
	void	start_frame();
	void	end_frame();
	void	draw(Scene* scene, int fps);
	bool	choose_tile(Scene* scene, Camera* cam, int id);
	bool	choose_ent_world_to_screen(Scene* scene, Camera* cam, float x, float y);
	void	edit_tilemap(Scene* scene);
	void	raycast_experimental(Scene* scene, Camera* cam, float x, float y);
	void	edit_target(Scene* scene);
	void	edit_target_tile(Scene* scene);
	void	recalc_proj(int width, int height);
	void	close();
private:
	void	save_scene(Scene* scene);
	void	save_scene_as(Scene* scene);
	void	save_map_as(Scene* scene);
};