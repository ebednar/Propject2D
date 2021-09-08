#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "render.h"
#include "events.h"
#include "skybox.h"
#include "scene.h"



class Engine {
public:
	Camera						cam;
	std::vector<Model *>		models;
	Scene						scene;
	Controls					controls;
	Skybox						skybox;
	Entity						*player;
	std::vector<text_t*>		text;
	double						delta_time = 0.0;
	bool						free_cam;
	bool						close_eng;
public:
			~Engine();
	void	init_engine(int width, int height);
	void	run_engine();
	void	add_model(Model *mod);
	void	add_entity(Entity *ent);
	void	set_player(Entity *ent);
	void	add_light_source(glm::vec3 l_pos, glm::vec3 color);
	void	add_text_ui(std::string str, float x, float y, float scale);
	void	change_text(std::string str, int id);
private:
	GLFWwindow				*window;
	Render					rend;
	ui_text					texter;
	double					old_time = 0.0;
	double					timer = 0.0;
	int						fps = 0;
	int						width = 0;
	int						height = 0;
};
