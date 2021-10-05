#include "engine.h"
#include "glad.h"
#include <iostream>

Engine::~Engine()
{
	int length = scene.model_atlas.size();
	for (auto mod : scene.model_atlas)
	{
		delete mod.second->vertices;
		delete mod.second;
	}
	length = scene.ents.size();
	for (auto ent : scene.ents)
		delete ent;
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetWindowUserPointer(window, &events);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	this->width = width;
	this->height = height;

	events.yaw = cam.yaw;
	events.pitch = cam.pitch;

	rend.init(static_cast<float>(width), static_cast<float>(height));
	
	skybox.init();
	skybox.set_shader("res/shaders/skybox_vert.glsl", "res/shaders/skybox_frag.glsl");
#ifdef EDITOR
	editorUI.init(window);
#endif
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
		fps_counter++;
		if (timer >= 1.0)
		{
			timer -= 1.0;
			fps = fps_counter;
			fps_counter = 0;
		}
		old_time = glfwGetTime();
		
		scene.update_scene();
		events_handling();

		//rend.draw_skybox(&skybox, &cam);
		rend.draw_tilemap(&scene, &cam);
		rend.draw_scene(&scene, &cam);
#ifdef EDITOR
		editorUI.draw(&scene, width, height, fps);
#endif // EDITOR

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
	if (events.keys[GLFW_KEY_UP])
		cam.pos.y += cam.speed;
	if (events.keys[GLFW_KEY_DOWN])
		cam.pos.y -= cam.speed;
	if (events.keys[GLFW_KEY_LEFT])
		cam.pos.x -= cam.speed;
	if (events.keys[GLFW_KEY_RIGHT])
		cam.pos.x += cam.speed;

	if (events.r_clicked)
	{
		cam.pos.x -= events.xoffset * 20.0f * delta_time;
		events.xoffset = 0;
		cam.pos.y -= events.yoffset * 20.0f * delta_time;
		events.yoffset = 0;
	}
	
	if (events.l_clicked)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
	}

	// camera rotation
	//cam.yaw = events.yaw;
	//cam.pitch = events.pitch;
	cam.update_free();

	if (events.resize)
	{
		width = events.width;
		height = events.height;
		rend.recalc_proj(width, height);
		glViewport(0, 0, width, height);
		events.resize = false;
	}
}

