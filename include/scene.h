#pragma once
#include "Entity.h"
#include <vector>
#include <string>
#include <map>

struct point_light
{
	glm::vec3	position;
	glm::vec3	color;
};

class Scene
{
public:
	std::map<std::string, Model*> model_atlas;
	std::map<std::string, std::string> vshader_atlas;
	std::map<std::string, std::string> fshader_atlas;
	std::map<std::string, std::string> material_atlas;
	std::vector<Entity>		ents;
	std::vector<point_light>	point_lights;
	unsigned int ents_numb = 0;
public:
	Scene();
	void	load_scene(const char* path);
	void	add_entity(Entity* ent);
	void	add_light_source(glm::vec3 l_pos, glm::vec3 color);
};