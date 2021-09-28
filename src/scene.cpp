#include <iostream>
#include <fstream>
#include "scene.h"
#include "shader.h"
#include "glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "player.h"
#include "obstacle.h"

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
	if (nrChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, text_data);
	else if (nrChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, text_width, text_height, 0, GL_RGB, GL_UNSIGNED_BYTE, text_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(text_data);
	texture_atlas[name] = texture;
}

int		Scene::load_resources(const char* path)
{
	std::ifstream	file;

	std::cout << "load resources " << path << std::endl;
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "resources file not found" << std::endl;
		return 1;
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
				load_texture(name, ("res/textures/" + text).c_str());
			}
		}
	}
	return 0;
}

int		Scene::load_map(const char* path)
{
	std::ifstream	file;

	std::cout << "load map " << path << std::endl;
	tilemap.shader_id = shader_atlas["base"];
	tilemap.texture_id = texture_atlas["wall"];
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "scene file not found" << std::endl;
		return 1;
	}
	std::string line;
	int x = 0;
	int y = 0;
	int raw_length;
	int column_length;
	std::getline(file, line);
	raw_length = std::stoi(line);
	std::getline(file, line);
	column_length = std::stoi(line);
	tilemap.tiles = new Tile[raw_length * column_length];
	while (std::getline(file, line))
	{
		std::string str = "";
		for (auto i : line)
		{
			if (i == ' ')
			{
				tilemap.tiles[y * raw_length + x].id = std::stoi(str);
				tilemap.tiles[y * raw_length + x].x = x;
				tilemap.tiles[y * raw_length + x].y = y;
				str = "";
				x++;
				tilemap.map_numb++;
			}
			str += i;
		}
		tilemap.tiles[y * raw_length + x].id = std::stoi(str);
		tilemap.tiles[y * raw_length + x].x = x;
		tilemap.tiles[y * raw_length + x].y = y;
		str = "";
		x = 0;
		y++;
		tilemap.map_numb++;
	}
	file.close();
	Model* mod = model_atlas["sprite"];
	tilemap.generate_batched_map(mod);
	return 0;
}

int		Scene::load_scene(const char* path)
{
	std::ifstream	file;

	std::cout << "load scene " << path << std::endl;
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "scene file not found" << std::endl;
		return 1;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line == "#player")
		{
			Player* player = new Player();

			std::getline(file, line);
			place_ent(player, line);
			add_entity(player);
		}
		if (line == "#obstacle")
		{
			Obstacle* obs = new Obstacle();

			std::getline(file, line);
			place_ent(obs, line);
			add_entity(obs);
		}
		if (line == "#light")
		{
			Light* light = new Light();

			std::getline(file, line);
			place_ent(light, line);
			add_entity(light);
			std::getline(file, line);
			read_light_info(light, line);
			light->scale(0.1f, 0.1f, 0.1f);
			point_lights.push_back(light);
		}
	}
	return 0;
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
				ent->move(0.0f, std::stof(str), 0.0f);
			count++;
			str = "";
			continue;
		}
		str += i;
	}
	ent->move(0.0f, 0.0f, std::stof(str));
	ent->rotate(0.0f, 0.0f, 0.0f);
}

void	Scene::read_light_info(Light* ent, std::string line)
{
	std::string str = "";
	int count = 0;
	for (auto i : line)
	{
		if (i == ' ')
		{
			if (count == 0)
				ent->color[0] = std::stof(str);
			else if (count == 1)
				ent->color[1] = std::stof(str);
			else if (count == 2)
				ent->color[2] = std::stof(str);
			else if (count == 3)
				ent->constant = std::stof(str);
			else if (count == 4)
				ent->linear = std::stof(str);
			count++;
			str = "";
			continue;
		}
		str += i;
	}
	ent->quadratic = std::stof(str);
}

void	Scene::add_entity(Entity* ent_ptr)
{
	ents.push_back(ent_ptr);
	ents_numb++;
}

void	Scene::update_scene()
{
	for (Entity* ent : ents)
	{
		ent->update();
	}
}