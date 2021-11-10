#pragma once
#include "scene.h"
#include "camera.h"
#include "ui_text.h"
#include "skybox.h"

enum class projection_type
{
	ortho,
	perspective
};

class Render
{
private:
	glm::mat4		projection;
	projection_type	proj_type = projection_type::ortho;
	unsigned int	framebuffer;
	unsigned int	colorBufferTexture;
	unsigned int	idBufferTexture;
	unsigned int	screenVBO;
	unsigned int	screenVAO;
	int				screen_shader;
public:
	~Render();
	void	init(projection_type type, float width = 640.0, float height = 480.0);
	void	resize(float width, float height);
	void	draw(Scene* scene, Camera* camera, Skybox* skybox, bool is_edit_tilemap = false);
	void	draw_target(Scene* scene, Camera* camera);
	int		read_pixel(int x, int y);
private:
	void	create_framebuffer(float width, float height);
	void	draw_scene(Scene* scene, Camera* camera);
	void	draw_skybox(Skybox* skybox, Camera* camera);
	void	draw_tilemap(Scene* scene, Camera* camera);
	void	draw_from_framebuffer();
};
