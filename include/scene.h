#pragma once
#include <vector>
#include <string>
#include <map>
#include "entity.h"
#include "light.h"
#include "tilemap.h"

class Scene
{
public:
	Tilemap								tilemap;
	std::map<std::string, Model*>		model_atlas;
	std::map<std::string, int>			shader_atlas;
	std::map<std::string, unsigned int>	texture_atlas;
	std::vector<Entity*>				ents;
	std::vector<Light*>					point_lights;
	Entity*								target = nullptr;
	unsigned int						ents_numb = 0;
	unsigned int						lights_numb = 0;
public:
	Scene();
	int		load_resources(const char* path);
	int		load_map(const char* path);
	int		load_scene(const char* path);
	void	add_entity(Entity* ent);
	void	add_point_light(Light* ent);
	void	update_scene();
private:
	void	load_texture(std::string name, const char* path);
	void	place_ent(Entity* ent, std::string line);
	void	read_light_info(Light* ent, std::string line);
};