#include "scene.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "shader.h"
#include "glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "entity/obstacle.h"
#include "entity/npc.h"

void	Scene::load_texture(std::string name, const char* path)
{
	int nrChannels;
	int text_width;
	int text_height;

	unsigned char* text_data = stbi_load(path, &text_width, &text_height, &nrChannels, 0);
	if (!text_data)
		std::cout << path << " - can't load texture file" << std::endl;

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
	texture_atlas[name].id = texture;
	texture_atlas[name].width = text_width;
	texture_atlas[name].height = text_height;
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
			while (line != "")
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
	}

	for (auto& i : std::filesystem::directory_iterator("res/textures/"))
	{
		std::string str = i.path().filename().string();
		std::string name = i.path().stem().string();
		if (!i.is_directory())
			load_texture(name, ("res/textures/" + str).c_str());
	}

	file.close();
	return 0;
}

int		Scene::load_map(const char* path)
{
	std::ifstream	file;

	std::cout << "load map " << path << std::endl;
	tilemap.shader_id = shader_atlas["base"];
	tilemap.texture_id = texture_atlas["tilemap"].id;
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "scene file not found" << std::endl;
		return 1;
	}
	std::string line;
	int x = 0;
	int y = 0;
	std::getline(file, line);
	tilemap.raws = std::stoi(line);
	std::getline(file, line);
	tilemap.columns = std::stoi(line);
	tilemap.tiles = new Tile[tilemap.raws * tilemap.columns];
	while (std::getline(file, line))
	{
		std::string str = "";
		for (auto i : line)
		{
			if (i == '/')
			{
				tilemap.tiles[y * tilemap.columns + x].idx = std::stoi(str);
				str = "";
			}
			else if (i == ' ')
			{
				tilemap.tiles[y * tilemap.columns + x].idy = std::stoi(str);
				tilemap.tiles[y * tilemap.columns + x].x = x;
				tilemap.tiles[y * tilemap.columns + x].y = y;
				str = "";
				x++;
				tilemap.tile_numb++;
			}
			else
				str += i;
		}
		tilemap.tiles[y * tilemap.columns + x].idy = std::stoi(str);
		tilemap.tiles[y * tilemap.columns + x].x = x;
		tilemap.tiles[y * tilemap.columns + x].y = y;
		str = "";
		x = 0;
		y++;
		tilemap.tile_numb++;
	}
	file.close();
	Model* mod = new Model(*model_atlas["sprite"]);
	tilemap.generate_batched_map(mod);
	return 0;
}

void	Scene::set_player(std::ifstream& file)
{
	std::string line;

	Player* player = new Player();

	add_entity(player);
	player->type = entity_type::Player;
	std::getline(file, line);
	while (line != "" && line != "//")
	{
		std::string data;
		std::getline(file, data);
		parse_ent_options(player, line, data);
		std::getline(file, line);
	}
	player->init_animations();
	this->player = player;
	player->game_input = game_input;
	player->others = &ents;

	player->set_subtexture(0, 0, player->sub_width, player->sub_height);
}

void	Scene::set_light(std::ifstream& file)
{
	std::string line;

	Light* light = new Light();

	add_entity(light);
	light->type = entity_type::Light;
	std::getline(file, line);
	while (line != "" && line != "//")
	{
		std::string data;
		std::getline(file, data);
		parse_ent_options(light, line, data);
		read_light_info(light, line, data);
		std::getline(file, line);
	}
	light->scale(0.1f, 0.1f, 0.1f);
	add_point_light(light);
}

void	Scene::set_obstacle(std::ifstream& file)
{
	std::string line;

	Obstacle* obs = new Obstacle();

	add_entity(obs);
	obs->type = entity_type::Obstacle;
	std::getline(file, line);
	while (line != "" && line != "//")
	{
		std::string data;
		std::getline(file, data);
		parse_ent_options(obs, line, data);
		std::getline(file, line);
	}
}

void	Scene::set_npc(std::ifstream& file)
{
	std::string line;

	Npc* npc = new Npc();

	add_entity(npc);
	npc->type = entity_type::Npc;
	std::getline(file, line);
	while (line != "" && line != "//")
	{
		std::string data;
		std::getline(file, data);
		parse_ent_options(npc, line, data);
		std::getline(file, line);
	}
	npc->set_subtexture(0, 0, 120, 80);
	npc->init_animations();

	npc->texter = this->texter;
}

int		Scene::load_scene(const char* path)
{
	std::ifstream	file;

	std::cout << "load scene " << path << std::endl;
	load_resources("res/scene/test.resources");
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "scene file not found" << std::endl;
		return 1;
	}
	scene_name = path;
	std::string line;
	while (std::getline(file, line))
	{
		if (line == "#map")
		{
			std::getline(file, line);
			map_name = line;
			std::string map_path("res/scene/");
			map_path += map_name;
			load_map(map_path.c_str());
		}
		else if (line == "#player")
		{
			set_player(file);
		}
		else if (line == "#npc")
		{
			set_npc(file);
		}
		else if (line == "#obstacle")
		{
			set_obstacle(file);
		}
		else if (line == "#light")
		{
			set_light(file);
		}
	}
	file.close();

	awake_scene();

	target = nullptr;
	is_loaded = true;
	return 0;
}