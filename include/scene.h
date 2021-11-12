#pragma once
#include <vector>
#include <string>
#include <map>
#include "entity/entity.h"
#include "entity/light.h"
#include "tilemap.h"

struct Image
{
	unsigned int	id;
	unsigned int	width;
	unsigned int	height;
};

class Scene
{
public:
	Tilemap								tilemap;
	std::map<std::string, Model*>		model_atlas;
	std::map<std::string, int>			shader_atlas;
	std::map<std::string, Image>		texture_atlas;
	std::vector<Entity*>				ents;
	std::vector<Light*>					point_lights;
	std::string							map_name = "";
	std::string							scene_name = "";
	Entity*								target = nullptr;
	Tile*								target_tile = nullptr;
	unsigned int						ents_numb = 0;
	unsigned int						lights_numb = 0;
	bool								is_loaded = false;
public:
	int		load_scene(const char* path);
	void	add_entity(Entity* ent);
	void	create_entity(entity_type type);
	void	destroy_entity(Entity* ent);
	void	add_point_light(Light* ent);
	int		save_scene(const char* path);
	void	update_scene();
	void	close_scene();
private:
	int		load_resources(const char* path);
	int		load_map(const char* path);
	void	load_texture(std::string name, const char* path);
	void	place_ent(Entity* ent, std::string line, std::string data);
	void	read_light_info(Light* ent, std::string line, std::string data);
};