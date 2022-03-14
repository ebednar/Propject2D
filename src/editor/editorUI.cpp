#include "editor/editorUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "editor/win_dialog.h"

#include <iostream>

void	EditorUI::init(GLFWwindow* window, int width, int height, bool scene_loaded, std::string scene_path)
{
	this->window = window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 100.0f);
	this->width = width;
	this->height = height;

	ent_types[0] = "Player";
	ent_types[1] = "Obstacle";
	ent_types[2] = "Light";

	save_enable = scene_loaded;
	if (scene_loaded)
		this->scene_path = scene_path;
}

void	EditorUI::start_frame()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void	EditorUI::end_frame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void	EditorUI::draw(Scene* scene, int fps)
{
	bool close = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				std::string filepath = open_file(window, "Scene file (*.scene)\0*.scene\0");
				if (!filepath.empty())
				{
					scene_path = filepath;
					save_enable = true;
					if (scene->is_loaded)
						scene->close_scene();
					scene->load_scene(filepath.c_str());
				}
			}
			if (ImGui::MenuItem("Save scene", NULL, false, save_enable))
			{
				save_scene(scene);
			}
			if (ImGui::MenuItem("Save scene as...", NULL))
			{
				save_scene_as(scene);
			}
			if (ImGui::MenuItem("Save map as...", NULL))
			{
				save_map_as(scene);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Main panel", &close))
	{
		ImGui::Text("FPS: %d", fps);
		ImGui::Text("Tilemap: %s", scene->map_name.c_str());
		ImGui::Text("Number of entities: %d", scene->ents_numb);
		ImGui::Text("Number of point lights: %d", scene->lights_numb);
		if (ImGui::Button("Edit tilemap"))
		{
			is_edit_tilemap = true;
			scene->target = nullptr;
		}
		if (ImGui::Button("Edit scene"))
		{
			is_edit_tilemap = false;
			scene->target = nullptr;
			scene->target_tile = nullptr;
		}
		if (ImGui::CollapsingHeader("Create entity"))
		{
			if (ImGui::Button("Create point light"))
			{
				scene->create_entity(entity_type::Light);
			}
			if (ImGui::Button("Create obstacle"))
			{
				scene->create_entity(entity_type::Obstacle);
			}
		}
		ImGui::End();
	}
}

void	EditorUI::save_scene(Scene* scene)
{
	scene->save_scene(scene_path.c_str());
}

void	EditorUI::save_scene_as(Scene* scene)
{
	save_map_as(scene);
	std::string filepath = save_file(window, "Scene file (*.scene)\0*.scene\0");
	if (!filepath.empty())
	{
		scene_path = filepath;
		save_enable = true;
		scene->save_scene(filepath.c_str());
	}
}

void	EditorUI::save_map_as(Scene* scene)
{
	std::string filepath_map = save_file(window, "Map file (*.map)\0*.map\0");
	if (!filepath_map.empty())
	{
		int i = 0, j = 0;
		for (auto c : filepath_map)
		{
			if (c == '\\')
				j = i;
			i++;

		}
		scene->map_name = filepath_map.substr(j + 1, filepath_map.length() - j);
		scene->tilemap.save_tilemap(scene->map_name);
	}
}

bool	EditorUI::choose_tile(Scene* scene, Camera* cam, int id)
{
	if (!is_edit_tilemap)
		return false;

	if (id >= 0 && id < scene->tilemap.tile_numb)
		scene->target_tile = &scene->tilemap.tiles[id];
	return false;
}

bool	EditorUI::choose_ent_world_to_screen(Scene* scene, Camera* cam, float x, float y)
{
	float nx = (x / (float)width) * 2.0f - 1.0f;
	float ny = -((y / (float)height) * 2.0f - 1.0f);

	int i = 0;
	for (i = 0; i < scene->ents_numb; ++i)
	{
		Entity* ent = scene->ents[i];
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, ent->position);
		model = glm::rotate(model, glm::radians(ent->angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(ent->angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(ent->angle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, ent->e_scale);

		glm::mat4 MVP = projection * cam->view * model;

		glm::vec4 ent_screen = MVP * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		ent_screen /= ent_screen.w;

		glm::vec4 aabb = glm::vec4(ent->position.x + 0.5f * ent->e_scale.x, ent->position.y + 0.5f * ent->e_scale.y, 0.0f, 1.0f);
		aabb = projection * cam->view * aabb;
		aabb /= aabb.w;

		bool check = false;
		if (aabb.x >= nx && aabb.x - 2 * (aabb.x - ent_screen.x) <= nx && aabb.y >= ny && aabb.y - 2 * (aabb.y - ent_screen.y) <= ny)
		{
			scene->target = ent;
			return true;
		}
	}
	return false;
}

void	EditorUI::edit_target(Scene* scene)
{
	if (!scene->target)
		return ;
	Entity* ent = scene->target;
	auto type = (int)ent->type;

	ImGui::Begin("Target editor");
	ImGui::Text("current entity type %s", ent_types[type].c_str());
	ImGui::Text("entity ID %d", ent->id);

	char bufx[64] = "";
	char bufy[64] = "";

	ImGui::Text("Position");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
	if (ImGui::Button("X##pos"))
		ent->position.x = 0.0f;
	ImGui::PopStyleColor(3);
	ImGui::SameLine(25.0f);
	ImGui::PushItemWidth(50.0f);
	ImGui::DragFloat("##X_pos", &ent->position.x, 0.1f);

	ImGui::SameLine(90.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
	if (ImGui::Button("Y##pos"))
		ent->position.y = 0.0f;
	ImGui::PopStyleColor(3);
	ImGui::SameLine(107.0f);
	ImGui::DragFloat("##Y_pos", &ent->position.y, 0.1f);

	ImGui::Text("Scale");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
	if (ImGui::Button("X##scale"))
		ent->e_scale.x = 1.0f;
	ImGui::PopStyleColor(3);
	ImGui::SameLine(25.0f);
	ImGui::PushItemWidth(50.0f);
	ImGui::DragFloat("##X_scale", &ent->e_scale.x, 0.1f);

	ImGui::SameLine(90.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
	if (ImGui::Button("Y##scale"))
		ent->e_scale.y = 1.0f;
	ImGui::PopStyleColor(3);
	ImGui::SameLine(107.0f);
	ImGui::DragFloat("##Y_scale", &ent->e_scale.y, 0.1f);

	ImGui::Text("SubText");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
	if (ImGui::Button("W##"))
		ent->sub_width = 512.0f;
	ImGui::PopStyleColor(3);
	ImGui::SameLine(25.0f);
	ImGui::PushItemWidth(50.0f);
	ImGui::DragInt("##Width_drag", &ent->sub_width, 1.0f);

	ImGui::SameLine(90.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
	if (ImGui::Button("H##"))
		ent->sub_height = 512.0f;
	ImGui::PopStyleColor(3);
	ImGui::SameLine(107.0f);
	ImGui::DragInt("##Height_drag", &ent->sub_height, 1.0f);

	if (ent->type == entity_type::Player)
	{
		ImGui::Text("Animation frames");
		ImGui::DragInt("##frame_count", &ent->animator.current_animation->frame_count, 1.0f, 1, 10);
		ImGui::Text("Animation duration");
		ImGui::DragFloat("##duration", &ent->animator.current_animation->duration, 0.1f, 0.1f, 10.0f);
	}
	else if (ent->type == entity_type::Obstacle)
	{

	}
	else if (ent->type == entity_type::Light)
	{
		for (Light* light : scene->point_lights)
		{
			if (light->id == ent->id)
			{
				ImGui::SliderFloat("red", &light->color[0], 0.0f, 1.0f);
				ImGui::SliderFloat("green", &light->color[1], 0.0f, 1.0f);
				ImGui::SliderFloat("blue", &light->color[2], 0.0f, 1.0f);
				ImGui::SliderFloat("constant", &light->constant, 0.1f, 1.5f);
				ImGui::SliderFloat("linear", &light->linear, 0.01f, 0.5f);
				ImGui::SliderFloat("quadratic", &light->quadratic, 0.001f, 0.05f);
			}
		}
	}
	if (ImGui::Button("Delete entity"))
	{
		scene->destroy_entity(scene->target);
		scene->target = nullptr;
	}
	ImGui::End();

	browser.draw(scene);
}

void	EditorUI::edit_tilemap(Scene* scene)
{
	static int new_raws = scene->tilemap.raws;
	static int new_columns = scene->tilemap.columns;
	ImGui::Begin("Tilemap editor");
	ImGui::Text("Change tilemap size");
	ImGui::PushItemWidth(100.0f);
	ImGui::InputInt("Raws", &new_raws, 1);
	ImGui::SameLine();
	ImGui::InputInt("Columns", &new_columns, 1);
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.8f, 1.0f });
	if (ImGui::Button("Generate new map"))
	{
		scene->tilemap.regenerate_new_map(scene->model_atlas["sprite"], new_raws, new_columns);
	}
	ImGui::PopStyleColor(1);
	ImGui::End();
	scene->tilemap.regenerate_map(scene->model_atlas["sprite"]);
}

void	EditorUI::edit_target_tile(Scene* scene)
{
	if (!scene->target_tile)
		return ;
	ImGui::Begin("Tile editor");
	ImGui::InputInt("Id", &scene->target_tile->id, 1);
	ImGui::End();
}

void	EditorUI::recalc_proj(int width, int height)
{
	projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 100.0f);
	this->width = width;
	this->height = height;
}

void	EditorUI::close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void	EditorUI::raycast_experimental(Scene* scene, Camera* cam, float x, float y)
{
	auto VP_mat = glm::inverse(projection * cam->view);
	auto ray_start = glm::vec4(x / width * 2.0f - 1.0f, -(y / height * 2.0f - 1.0f), -0.8f, 1.0f);
	auto ray_end = glm::vec4(x / width * 2.0f - 1.0f, -(y / height * 2.0f - 1.0f), 0.0f, 1.0f);
	auto ray_start_world = VP_mat * ray_start;
	auto ray_end_world = VP_mat * ray_end;
	ray_start_world /= ray_start_world.w;
	ray_end_world /= ray_end_world.w;

	glm::vec3 ray_dir(ray_end_world - ray_start_world);
	ray_dir = glm::normalize(ray_dir);
	glm::vec3 ray_origin(ray_start_world);
	ray_origin = glm::normalize(ray_origin);

	float tMin = 0.0f;
	float tMax = 100000.0f;

	Entity* ent = scene->ents[0];
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, ent->position);
	model = glm::rotate(model, glm::radians(ent->angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ent->angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ent->angle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, ent->e_scale);

	glm::vec3 player_pos(model[3].x, model[3].y, model[3].z);
	glm::vec3 delta = player_pos - ray_origin;

	bool check;

	{
		glm::vec3 xaxis(model[0].x, model[0].y, model[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_dir, xaxis);

		if (fabs(f) > 0.001f)
		{
			float t1 = (e + (-0.5f)) / f;
			float t2 = (e + (0.5f)) / f;

			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			if (tMax < tMin)
				check = false;
			else
				check = true;
		}
		else
		{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + (-0.5f) > 0.0f || -e + 0.5f < 0.0f)
				check = false;
			else
				check = true;
		}

	}

	int c = check;
}