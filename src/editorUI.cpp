#include "editorUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void	EditorUI::init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void	EditorUI::draw(Scene* scene, int width, int height, int fps)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Light debug");
	ImGui::Text("window resolution: %d %d", width, height);
	ImGui::Text("fps: %d", fps);
	ImGui::SliderFloat("red", &scene->point_lights[0]->color[0], 0.0f, 1.0f);
	ImGui::SliderFloat("green", &scene->point_lights[0]->color[1], 0.0f, 1.0f);
	ImGui::SliderFloat("blue", &scene->point_lights[0]->color[2], 0.0f, 1.0f);
	ImGui::SliderFloat("constant", &scene->point_lights[0]->constant, 0.1f, 1.5f);
	ImGui::SliderFloat("linear", &scene->point_lights[0]->linear, 0.01f, 0.5f);
	ImGui::SliderFloat("quadratic", &scene->point_lights[0]->quadratic, 0.001f, 0.05f);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void	EditorUI::close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}