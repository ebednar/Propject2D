#include <iostream>
#include "entity/obstacle.h"
#include "entity/npc.h"
#include "scene.h"
#include "ui_text.h"

void	Scene::create_entity(entity_type type)
{
	if (type == entity_type::Light)
	{
		Light* light = new Light();
		add_entity(light);
		light->set_model(model_atlas["sprite"]);
		light->material.set_shader(shader_atlas["light"]);
		light->material.set_texture(texture_atlas["black_mage"].id, 512, 512);
		light->scale(0.1f, 0.1f, 0.1f);
		light->type = entity_type::Light;
		add_point_light(light);
	}
	else
	{
		Entity* ent = nullptr;
		if (type == entity_type::Npc)
		{
			ent = new Npc();
			ent->type = entity_type::Npc;
		}
		else // Obstacle
		{
			ent = new Obstacle();	
			ent->type = entity_type::Obstacle;
		}
		add_entity(ent);
		ent->set_model(model_atlas["sprite"]);
		ent->material.set_shader(shader_atlas["base"]);
		ent->material.set_texture(texture_atlas["black_mage"].id, 512, 512);
	}
}

void	Scene::add_entity(Entity* ent_ptr)
{
	ents.push_back(ent_ptr);
	ent_ptr->id = ents_numb;
	ents_numb++;
}

void	Scene::add_point_light(Light* ent_ptr)
{
	point_lights.push_back(ent_ptr);
	lights_numb++;
}

void	Scene::destroy_entity(Entity* ent)
{
	if (ent->type == entity_type::Light)
	{
		if (lights_numb == 1)
		{
			std::cout << "can not delete the last light source" << std::endl;
			return;
		}
		for (int i = 0; i < lights_numb; ++i)
		{
			if (point_lights[i]->id == ent->id)
			{
				point_lights.erase(point_lights.begin() + i);
				break;
			}
		}
	}
	for (int i = 0; i < ents_numb; ++i)
	{
		if (ents[i]->id == ent->id)
		{
			delete ents[i];
			ents.erase(ents.begin() + i);
			break;
		}
	}
	ents_numb--;
}

void	Scene::awake_scene()
{
	for (Entity* ent : ents)
	{
		ent->awake();
	}
}

void	Scene::update_scene()
{
	for (Entity* ent : ents)
	{
		ent->update_dist(player);
		ent->play_animation();
		ent->update();
	}
}

void	Scene::close_scene()
{
	std::cout << "close scene " << scene_name << std::endl;
	target = nullptr;
	delete[] tilemap.tiles;
	tilemap.tiles = nullptr;
	delete[] tilemap.mod.vertices;
	tilemap.mod.vertices = nullptr;
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
	map_name = "";
	ents_numb = 0;
	lights_numb = 0;
	is_loaded = false;
}