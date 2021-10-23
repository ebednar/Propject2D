#include <iostream>
#include <fstream>
#include <sstream>
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
	file.close();
	return 0;
}

int		Scene::load_map(const char* path)
{
	std::ifstream	file;

	std::cout << "load map " << path << std::endl;
	tilemap.shader_id = shader_atlas["base"];
	tilemap.texture_id = texture_atlas["tileset"];
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
			if (i == ' ')
			{
				tilemap.tiles[y * tilemap.raws + x].id = std::stoi(str);
				tilemap.tiles[y * tilemap.raws + x].x = x;
				tilemap.tiles[y * tilemap.raws + x].y = y;
				str = "";
				x++;
				tilemap.tile_numb++;
			}
			str += i;
		}
		tilemap.tiles[y * tilemap.raws + x].id = std::stoi(str);
		tilemap.tiles[y * tilemap.raws + x].x = x;
		tilemap.tiles[y * tilemap.raws + x].y = y;
		str = "";
		x = 0;
		y++;
		tilemap.tile_numb++;
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
	load_resources("res/scene/test.resources");
	file.open(path);
	if (not file.is_open())
	{
		std::cout << "scene file not found" << std::endl;
		return 1;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line == "#map")
		{
			std::getline(file, line);
			name = line;
		}
		if (line == "#player")
		{
			Player* player = new Player();

			std::getline(file, line);
			add_entity(player);
			place_ent(player, line);
			player->type = entity_type::Player;
		}
		if (line == "#obstacle")
		{
			Obstacle* obs = new Obstacle();

			std::getline(file, line);
			add_entity(obs);
			place_ent(obs, line);
			obs->type = entity_type::Obstacle;
		}
		if (line == "#light")
		{
			Light* light = new Light();

			std::getline(file, line);
			add_entity(light);
			place_ent(light, line);
			std::getline(file, line);
			read_light_info(light, line);
			light->scale(0.1f, 0.1f, 0.1f);
			light->type = entity_type::Light;
			add_point_light(light);
		}
	}
	file.close();
	std::string map_path("res/scene/");
	map_path += name;
	load_map(map_path.c_str());
	target = nullptr;
	is_loaded = true;
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
	ent_ptr->id = ents_numb;
	ents_numb++;
}

void	Scene::create_entity(entity_type type)
{
	if (type == entity_type::Obstacle)
	{
		Obstacle* obs = new Obstacle();
		add_entity(obs);
		obs->set_model(model_atlas["sprite"]);
		obs->material.set_shader(shader_atlas["base"]);
		obs->material.set_texture(texture_atlas["player"]);
		obs->type = entity_type::Obstacle;
	}
	else if (type == entity_type::Light)
	{
		Light* light = new Light();
		add_entity(light);
		light->set_model(model_atlas["sprite"]);
		light->material.set_shader(shader_atlas["light"]);
		light->material.set_texture(texture_atlas["player"]);
		light->scale(0.1f, 0.1f, 0.1f);
		light->type = entity_type::Light;
		add_point_light(light);
	}
}
void	Scene::add_point_light(Light* ent_ptr)
{
	point_lights.push_back(ent_ptr);
	lights_numb++;
}

int		Scene::save_scene(const char* path)
{
	std::stringstream out;
	std::cout << "saving scene " << path << std::endl;

	out << "#map\n" << name << '\n';
	for (Entity* ent : ents)
	{
		if (ent->type == entity_type::Player)
		{
			out << "#player\n";
			out << "sprite base player " << ent->position.x << ' ' << ent->position.y << ' ' << ent->position.z << '\n';
		}
		else if (ent->type == entity_type::Obstacle)
		{
			out << "#obstacle\n";
			out << "sprite base player " << ent->position.x << ' ' << ent->position.y << ' ' << ent->position.z << '\n';
		}
		else if (ent->type == entity_type::Light)
		{
			out << "#light\n";
			out << "cube light player " << ent->position.x << ' ' << ent->position.y << ' ' << ent->position.z << '\n';
			
			for (Light* light : point_lights)
				if (ent->id == light->id)
				{
					out << light->color[0] << ' ' << light->color[1] << ' ' << light->color[2] << ' ' << light->constant << ' ' << light->linear << ' ' << light->quadratic << '\n';
					break;
				}
		}
	}

	std::ofstream	file(path);
	file << out.str();
	file.close();
	return 0;
};

void	Scene::update_scene()
{
	for (Entity* ent : ents)
	{
		ent->update();
	}
}

void	Scene::close_scene()
{
	target = nullptr;
	delete[] tilemap.tiles;
	tilemap.tiles = nullptr;
	tilemap.tile_numb = 0;
	tilemap.shader_id = 0;
	tilemap.texture_id = 0;
	for (auto mod : model_atlas)
	{
		delete mod.second->vertices;
		delete mod.second;
	}
	model_atlas.clear();
	texture_atlas.clear();
	for (auto ent : ents)
		delete ent;
	ents.clear();
	for (auto light : point_lights)
		light = nullptr;
	point_lights.clear();
	name = "";
	ents_numb = 0;
	lights_numb = 0;
	is_loaded = false;
}