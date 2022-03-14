#include "scene.h"
#include "editor/content_browser.h"
#include "imgui.h"

#include <filesystem>
#include <string>

void ContentBrowser::draw(Scene* scene)
{
	if (ImGui::Begin("Content browser"))
	{
		for (auto& i : std::filesystem::directory_iterator("res/textures/"))
		{
			std::string str = i.path().stem().string();
			if (ImGui::Button(str.c_str()))
			{
				scene->target->material.texture_name = str;
				scene->target->material.texture_id = scene->texture_atlas[str].id;
				scene->target->material.text_width = scene->texture_atlas[str].width;
				scene->target->material.text_height = scene->texture_atlas[str].height;
				scene->target->set_subtexture(0, 0);
				//scene->target->scale(scene->target->material.text_width / 512.0f, scene->target->material.text_height / 512.0f, 1.0f);
			}
		}


		ImGui::End();
	}
}
