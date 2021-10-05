#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "render.h"
#include "events.h"
#include "skybox.h"
#include "scene.h"
#include "editorUI.h"

#define EDITOR

class Engine {
public:
	Camera						cam;
	Scene						scene;
	Events						events;
	Skybox						skybox;
	EditorUI					editorUI;
	std::vector<text_t*>		text;
	double						delta_time = 0.0;
	bool						close_eng = false;
public:
			~Engine();
	void	init_engine(int width, int height);
	void	run_engine();
	void	add_text_ui(std::string str, float x, float y, float scale);
	void	change_text(std::string str, int id);
private:
	GLFWwindow				*window;
	Render					rend;
	ui_text					texter;
	double					old_time = 0.0;
	double					timer = 0.0;
	int						fps_counter = 0;
	int						fps = 0;
	int						width = 0;
	int						height = 0;
private:
	void	events_handling();
};
