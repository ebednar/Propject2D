#include "engine.h"
#include "glad.h"
#include <iostream>

Engine::~Engine()
{
	std::cout << "Engine off" << std::endl;
}

void Engine::init_engine(const char* name, int width, int height)
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, name, NULL, NULL);
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
#ifndef EDITOR
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif // !EDITOR
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetWindowUserPointer(window, &events);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	this->width = width;
	this->height = height;

	events.yaw = camera.yaw;
	events.pitch = camera.pitch;

	render.init(projection_type::perspective, static_cast<float>(width), static_cast<float>(height));
	
	/*skybox.init();
	skybox.set_shader("res/shaders/skybox.vert", "res/shaders/skybox.frag");*/

}

void Engine::run_engine()
{
	old_time = glfwGetTime();
#ifdef EDITOR
	editorUI.init(window, width, height, scene.is_loaded, "res/scene/test_saved.scene");
#endif

	scene.awake_scene();

	while (!glfwWindowShouldClose(window))
	{
		delta_time = glfwGetTime() - old_time;
		timer += delta_time;
		fps_counter++;
		if (timer >= 1.0)
		{
			timer -= 1.0;
			fps = fps_counter;
			fps_counter = 0;
		}
		old_time = glfwGetTime();

		scene.update_scene();

		render.draw(&scene, &camera, &skybox, editorUI.is_edit_tilemap);

	#ifdef EDITOR
		if (scene.target)
			render.draw_target(&scene, &camera);
		editorUI.start_frame();
		editorUI.edit_target(&scene);
		editorUI.edit_target_tile(&scene);
		editorUI.draw(&scene, fps);
		if (editorUI.is_edit_tilemap)
		{
			editorUI.edit_tilemap(&scene);
		}
		editorUI.end_frame();
	#endif // EDITOR

		events_handling();
		glfwSwapBuffers(window);
		glfwPollEvents();
		if(close_eng)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	editorUI.close();
	scene.close_scene();
	glfwDestroyWindow(window);
	glfwTerminate();
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

void	Engine::events_handling()
{
	camera.speed = 8.0f * delta_time;
	mouse_speed = 15.0f * delta_time;
	scroll_speed = 20.0f * delta_time;

#ifdef EDITOR

	if (events.keys[GLFW_KEY_UP])
		camera.pos.z += camera.speed;
	if (events.keys[GLFW_KEY_DOWN])
		camera.pos.z -= camera.speed;
	if (events.keys[GLFW_KEY_LEFT])
		camera.pos.x -= camera.speed;
	if (events.keys[GLFW_KEY_RIGHT])
		camera.pos.x += camera.speed;

	// hold right mouse to scroll screen
	if (events.r_clicked)
	{
		camera.pos.x -= events.xoffset * scroll_speed;
		camera.pos.y -= events.yoffset * scroll_speed;
	}
	
	// click left mouse to choose entity
	static bool targeted = false;
	if (events.l_clicked)
	{
		editorUI.choose_tile(&scene, &camera, render.read_pixel(events.last_x, height - events.last_y));
	}
	if (events.l_clicked && !editorUI.is_edit_tilemap)
	{
		targeted = editorUI.choose_ent_world_to_screen(&scene, &camera, events.last_x, events.last_y);
		events.l_hold = true;
		events.l_clicked = false;
	}

	// hold left mouse to move entity
	if (events.l_hold && targeted)
	{
		scene.target->move(events.xoffset * mouse_speed, events.yoffset * mouse_speed, 0.0f);
	}

	// release left mouse
	if (events.l_released)
	{
		targeted = false;
		events.l_released = false;
		events.l_hold = false;
	}

#endif // EDITOR

	// cameraera rotation
	//camera.yaw = events.yaw;
	//camera.pitch = events.pitch;
	camera.update_free();

	if (events.resize)
	{
		width = events.width;
		height = events.height;
		render.resize(width, height);
		editorUI.recalc_proj(width, height);
		glViewport(0, 0, width, height);
		events.resize = false;
	}

	events.xoffset = 0;
	events.yoffset = 0;
}

