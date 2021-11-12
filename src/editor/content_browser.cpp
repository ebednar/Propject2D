#include "editor/content_browser.h"
#include "imgui.h"

void ContentBrowser::draw()
{
	if (ImGui::Begin("Content browser"))
	{
		ImGui::Text("test");
		ImGui::End();
	}
}
