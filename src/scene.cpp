#include "scene.h"
#include <iostream>

Scene::Scene()
{
	
}

void	Scene::load_scene(const char* path)
{
	std::cout << "load scene " << path << std::endl;
}

void	Scene::add_entity(Entity* ent_ptr)
{
	ents.push_back(*ent_ptr);
	ents_numb++;
}

void	Scene::add_light_source(glm::vec3 l_pos, glm::vec3 color)
{
	point_lights.push_back({ l_pos, color });
}
