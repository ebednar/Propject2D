#pragma once
#include "entity.h"
#include "player.h"
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
	std::map<std::string, int> shader_atlas;
	std::map<std::string, unsigned int> texture_atlas;
	std::vector<Entity*>		ents;
	std::vector<point_light>	point_lights;
	unsigned int ents_numb = 0;
public:
	Scene();
	int		load_resources(const char* path);
	int		load_scene(const char* path);
	void	load_texture(std::string name, const char* path);
	void	add_entity(Entity* ent);
	void	add_light_source(glm::vec3 l_pos, glm::vec3 color);
	void	update_scene();
	void	place_ent(Entity* ent, std::string line);
};