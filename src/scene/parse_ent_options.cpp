#include "scene.h"

void static	parse_animation(Entity* ent, std::string line, std::string data, unsigned int texture_id, int index)
{
	std::string str = "";
	int count = 0;
	int frame_count = 1;
	for (auto i : data)
	{
		if (i == ' ')
		{
			if (count == 0)
				frame_count = std::stoi(str);
			count++;
			str = "";
			continue;
		}
		str += i;
	}
	float duration = std::stof(str);

	ent->animator.animations[index].init(texture_id, frame_count, duration);
}

void	Scene::parse_ent_options(Entity* ent, std::string line, std::string data)
{
	if (line == "-model:")
	{
		ent->set_model(model_atlas[data]);
	}
	else if (line == "-shader:")
	{
		ent->material.set_shader(shader_atlas[data]);
	}
	else if (line == "-texture:")
	{
		ent->material.set_texture(texture_atlas[data].id, texture_atlas[data].width, texture_atlas[data].height);
		ent->material.texture_name = data;
	}
	else if (line == "-position:")
	{
		std::string str = "";
		int count = 0;
		for (auto i : data)
		{
			if (i == ' ')
			{
				if (count == 0)
					ent->move_to(std::stof(str), 0.0f, 0.0f);
				else if (count == 1)
					ent->move(0.0f, std::stof(str), 0.0f);
				count++;
				str = "";
				continue;
			}
			str += i;
		}
		ent->move(0.0f, 0.0f, std::stof(str));
	}
	else if (line == "-subtexture size:")
	{
		std::string str = "";
		int count = 0;
		for (auto i : data)
		{
			if (i == ' ')
			{
				if (count == 0)
					ent->sub_width = std::stoi(str);
				count++;
				str = "";
				continue;
			}
			str += i;
		}
		ent->sub_height = std::stoi(str);

		// TODO: scale to 512
		//ent->scale(ent->sub_width / 512.0f, ent->sub_height / 512.0f, 1.0f);
		//ent->scale(ent->material.text_width / 512.0f, ent->material.text_height / 512.0f, 1.0f);
	}
	else if (line == "-animation idle:")
	{
		parse_animation(ent, line, data, texture_atlas["p_idle"].id, 0);
	}
	else if (line == "-animation walk:")
	{
		parse_animation(ent, line, data, texture_atlas["p_walk"].id, 1);
	}
}

void	Scene::read_light_info(Light* ent, std::string line, std::string data)
{
	if (line == "-lightning:")
	{
		std::string str = "";
		int count = 0;
		for (auto i : data)
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
}