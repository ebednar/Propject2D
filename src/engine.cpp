#include "engine.h"
#include "glad.h"
#include <iostream>

Engine::~Engine()
{
	scene.close_scene();
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	/*glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/
	this->width = width;
	this->height = height;

	events.yaw = cam.yaw;
	events.pitch = cam.pitch;

	rend.init(static_cast<float>(width), static_cast<float>(height));
	
	/*skybox.init();
	skybox.set_shader("res/shaders/skybox_vert.glsl", "res/shaders/skybox_frag.glsl");*/
#ifdef EDITOR
	editorUI.init(window, width, height);
#endif
}

void Engine::run_engine()
{
	old_time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);

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
		
		//rend.draw_skybox(&skybox, &cam);
		if (scene.is_loaded)
		{
			scene.update_scene();
			rend.draw_tilemap(&scene, &cam);
			if (!editorUI.is_edit_tilemap)
				rend.draw_scene(&scene, &cam);
		}
	#ifdef EDITOR
		if (scene.target)
			rend.draw_target(&scene, &cam);
		editorUI.start_frame();
		editorUI.edit_target(&scene);
		editorUI.draw(&scene, fps);
		if (editorUI.is_edit_tilemap)
			editorUI.edit_tilemap(&scene);
		editorUI.end_frame();
	#endif // EDITOR

		events_handling();
		glfwSwapBuffers(window);
		glfwPollEvents();
		if(close_eng)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	editorUI.close();
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
	cam.speed = 8.0f * delta_time;
	mouse_speed = 15.0f * delta_time;

#ifdef EDITOR

	if (events.keys[GLFW_KEY_UP])
		cam.pos.z += cam.speed;
	if (events.keys[GLFW_KEY_DOWN])
		cam.pos.z -= cam.speed;
	if (events.keys[GLFW_KEY_LEFT])
		cam.pos.x -= cam.speed;
	if (events.keys[GLFW_KEY_RIGHT])
		cam.pos.x += cam.speed;

	// hold right mouse to scroll screen
	if (events.r_clicked)
	{
		cam.pos.x -= events.xoffset * mouse_speed;
		cam.pos.y -= events.yoffset * mouse_speed;
	}
	
	// click left mouse to choose entity
	static bool targeted = false;
	if (events.l_clicked && !editorUI.is_edit_tilemap)
	{
		targeted = editorUI.choose_ent(&scene, &cam, events.last_x, events.last_y);
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

	// camera rotation
	//cam.yaw = events.yaw;
	//cam.pitch = events.pitch;
	cam.update_free();

	if (events.resize)
	{
		width = events.width;
		height = events.height;
		rend.recalc_proj(width, height);
		editorUI.recalc_proj(width, height);
		glViewport(0, 0, width, height);
		events.resize = false;
	}

	events.xoffset = 0;
	events.yoffset = 0;
}

