#pragma once
#include "scene.h"
#include "camera.h"
#include "ui_text.h"
#include "skybox.h"
struct vec3
{
	float x;
	float y;
	float z;
};

class Render
{
public:
	Entity *player;
public:
	void	init(float width = 640.0, float height = 480.0);
	void	draw_scene(Scene* scene, Camera* cam);
	void	draw_skybox(Skybox *skybox, Camera* cam);

private:
	glm::mat4	projection;
	glm::mat4	ortProjection;
	vec3		light_pos[3];
};
