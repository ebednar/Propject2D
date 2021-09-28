#pragma once
#include "scene.h"
#include "camera.h"
#include "ui_text.h"
#include "skybox.h"

class Render
{
public:
	void	init(float width = 640.0, float height = 480.0);
	void	draw_scene(Scene* scene, Camera* cam);
	void	draw_skybox(Skybox *skybox, Camera* cam);
	void	draw_tilemap(Scene* scene, Camera* cam);
	void	recalc_proj(float width, float height);
private:
	glm::mat4	projection;
	glm::mat4	ortProjection;
};
