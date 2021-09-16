#include <iostream>
#include <fstream>
#include "scene.h"
#include "shader.h"
#include "glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Scene::Scene()
{
	
}

void	Scene::load_texture(std::string name, const char* path)
{
	int nrChannels;
	int text_width;
	int text_height;

	unsigned char* text_data = stbi_load(path, &text_width, &text_height, &nrChannels, 0);
	if (!text_data)
		std::cout << "can't load texture file" << std::endl;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, text_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(text_data);
	texture_atlas[name] = texture;
}

int		Scene::load_resources(const char* path)
{
	std::ifstream		file;

	std::cout << "load resources " << path << std::endl;
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "resources file not found" << std::endl;
		return (1);
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line == "#models:")
		{
			std::getline(file, line);
			while (line != "#shaders:")
			{
				if (line == "//")
				{
					std::getline(file, line);
					continue;
				}
				std::string name;
				std::string obj;
				name = line;
				std::getline(file, obj);
				Model* mod = new Model();
				mod->load_obj(("res/models/" + obj).c_str(), true);
				model_atlas[name] = mod;
				std::getline(file, line);
			}
		}
		if (line == "#shaders:")
		{
			std::getline(file, line);
			while (line != "#texture:")
			{
				if (line == "//")
				{
					std::getline(file, line);
					continue;
				}
				std::string name;
				std::string vshader;
				std::string fshader;
				name = line;
				std::getline(file, vshader);
				std::getline(file, fshader);
				Model* mod = new Model();
				int shader;
				create_shader(&shader, ("res/shaders/" + vshader).c_str(), ("res/shaders/" + fshader).c_str());
				shader_atlas[name] = shader;
				std::getline(file, line);
			}
		}
		if (line == "#texture:")
		{		
			while (std::getline(file, line))
			{
				if (line == "//")
				{
					continue;
				}
				std::string name;
				std::string text;
				name = line;
				std::getline(file, text);
				Model* mod = new Model();
				load_texture(name, ("res/textures/" + text).c_str());
			}
		}
	}
}

int		Scene::load_scene(const char* path)
{
	std::ifstream		file;

	std::cout << "load scene " << path << std::endl;
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "scene file not found" << std::endl;
		return (1);
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line == "#player")
		{
			Entity* player = new Player();

			std::getline(file, line);
			place_ent(player, line);
		}
	}
}

void	Scene::place_ent(Entity* ent, std::string line)
{
	std::string str = "";

	int count = 0;
	for (auto i : line)
	{
		if (i == ' ')
		{
			if (count == 0)
				ent->set_model(model_atlas[str]);
			else if (count == 1)
				ent->material.set_shader(shader_atlas[str]);
			else if (count == 2)
				ent->material.set_texture(texture_atlas[str]);
			else if (count == 3)
				ent->move_to(std::stof(str), 0.0f, 0.0f);
			else if (count == 4)
				ent->move_to(0.0f, std::stof(str), 0.0f);
			else if (count == 5)
				ent->move_to(0.0f, 0.0f, std::stof(str));
			count++;
			str = "";
			continue;
		}
		str += i;
	}
	ent->rotate(0.0f, 0.0f, 0.0f);
	add_entity(ent);
}

void	Scene::add_entity(Entity* ent_ptr)
{
	ents.push_back(ent_ptr);
	ents_numb++;
}

void	Scene::add_light_source(glm::vec3 l_pos, glm::vec3 color)
{
	point_lights.push_back({ l_pos, color });
}

void	Scene::update_scene()
{
	for (Entity* ent : ents)
	{
		ent->update();
	}
}