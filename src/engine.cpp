#include "engine.h"
#include "glad.h"
#include <iostream>
#include "glm/glm.hpp"


Engine::~Engine()
{
	/*int length = models.size();
	for (int i = 0; i < length; ++i)
	{
		delete models[i]->vertices;
		delete models[i];
	}*/
	//int length = scene.ents.size();
	//for (int i = 0; i < length; ++i)
	//	delete scene.ents[i];
	std::cout << "Engine off" << std::endl;
}

void Engine::init_engine(int width, int height)
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "42 run", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowUserPointer(window, &controls);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->width = width;
	this->height = height;

	controls.yaw = cam.yaw;
	controls.pitch = cam.pitch;

	rend.init(static_cast<float>(width), static_cast<float>(height));
	
	skybox.init();
	skybox.set_shader("res/shaders/skybox_vert.glsl", "res/shaders/skybox_frag.glsl");
}

void Engine::run_engine()
{
	old_time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);

		delta_time = glfwGetTime() - old_time;
		timer += delta_time;
		fps++;
		if (timer >= 1.0)
		{
			timer -= 1.0;
			std::cout << "fps - " << fps << std::endl;
			fps = 0;
		}
		old_time = glfwGetTime();

		cam.speed = 8.0f * delta_time;
		if (controls.keys[GLFW_KEY_UP])
			cam.pos.y += cam.speed;
		if (controls.keys[GLFW_KEY_DOWN])
			cam.pos.y -= cam.speed;
		if (controls.keys[GLFW_KEY_LEFT])
			cam.pos.x -= cam.speed;
		if (controls.keys[GLFW_KEY_RIGHT])
			cam.pos.x += cam.speed;
		cam.yaw = controls.yaw;
		cam.pitch = controls.pitch;

		cam.update_free();
		rend.draw_skybox(&skybox, &cam);
		rend.draw_scene(&scene, &cam);

		glfwSwapBuffers(window);
		glfwPollEvents();
		if(close_eng)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	glfwTerminate();
}

void Engine::add_entity(Entity *ent_ptr)
{
	scene.add_entity(ent_ptr);
}

void Engine::set_player(Entity *ent)
{
	player = ent;
	rend.player = ent;
}

void Engine::add_light_source(glm::vec3 l_pos, glm::vec3 color)
{
	scene.add_light_source(l_pos, color);
}

void	 Engine::add_text_ui(std::string str, float x, float y, float scale)
{
	text_t *txt = new text_t(str, x, y, scale);
	text.push_back(txt);
}

void	 Engine::change_text(std::string str, int id)
{
	text[id]->str = str;
}

