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
	std::map<std::string, std::string> models;
	std::map<std::string, std::string> vshaders;
	std::map<std::string, std::string> fshaders;
	std::map<std::string, std::string> materials;
	std::vector<Entity*>		ents;
	std::vector<point_light>	point_lights;
public:
	void	load_scene(const char* path);
	void	add_entity(Entity* ent);
	void	add_light_source(glm::vec3 l_pos, glm::vec3 color);
};