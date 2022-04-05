#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "render.h"
#include "events.h"
#include "skybox.h"
#include "scene.h"
#include "editor/editorUI.h"
#include "timer.h"

#define EDITOR

class Engine {
public:
	Camera						camera;
	Scene						scene;
	Events						events;
	Skybox						skybox;
	Timer						game_timer;
	EditorUI					editorUI;
	bool						close_eng = false;
public:
			~Engine();
	void	init_engine(const char* name, int width, int height);
	void	run_engine();
	void	add_text_ui(std::string str, float x, float y, float scale);
	void	change_text(std::string str, int id);
private:
	GLFWwindow				*window;
	Render					render;
	Ui_text					texter;
	double					old_time = 0.0;
	double					timer = 0.0;
	float					mouse_speed = 0.0f;
	float					scroll_speed = 0.0f;
	int						fps_counter = 0;
	int						fps = 0;
	int						width = 0;
	int						height = 0;
private:
	void	events_handling();
};
