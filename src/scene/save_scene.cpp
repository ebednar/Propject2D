#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "scene.h"

static void	print_entity_info_in_file(const Entity* ent, std::stringstream& out)
{
	out << "-model:\nsprite\n-shader:\nbase\n-texture:\n" << ent->material.texture_name 
		<< "\n-position:\n" << ent->position.x << ' ' << ent->position.y << ' ' << ent->position.z 
		<< "\n-subtexture size:\n" << ent->sub_width << ' ' << ent->sub_height << '\n';
}

static void	print_player_info_in_file(const Entity* ent, std::stringstream& out)
{
	out << "-animation idle:\n" << ent->animator.animations[0].frame_count << ' ' << ent->animator.animations[0].duration
		<< "\n-animation walk:\n" << ent->animator.animations[1].frame_count << ' ' << ent->animator.animations[1].duration << '\n';
}

int		Scene::save_scene(const char* path)
{
	std::stringstream out;
	std::cout << "saving scene " << path << std::endl;

	tilemap.save_tilemap(map_name);
	out << "#map\n" << map_name << '\n';
	for (Entity* ent : ents)
	{
		if (ent->type == entity_type::Player)
		{
			out << "//\n#player\n";
			print_entity_info_in_file(ent, out);
			print_player_info_in_file(ent, out);
		}
		else if (ent->type == entity_type::Obstacle)
		{
			out << "//\n#obstacle\n";
			print_entity_info_in_file(ent, out);
		}
		else if (ent->type == entity_type::Light)
		{
			out << "//\n#light\n";
			out << "-model:\nsprite\n-shader:\nlight\n-texture:\n" << ent->material.texture_name << "\n-position:\n" << ent->position.x << ' ' << ent->position.y << ' ' << ent->position.z << '\n';

			for (Light* light : point_lights)
				if (ent->id == light->id)
				{
					out << "-lightning:\n" << light->color[0] << ' ' << light->color[1] << ' ' << light->color[2] << ' ' << light->constant << ' ' << light->linear << ' ' << light->quadratic << '\n';
					break;
				}
		}
	}

	std::ofstream	file(path);
	file << out.str();
	file.close();
	return 0;
};
