#include "render.h"
#include "glad.h"
#include <string>
#include <iostream>
#include "shader.h"

#define EDITOR

void Render::init(float width, float height)
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	create_shader(&screen_shader, "res/shaders/screen_vs.glsl", "res/shaders/screen_fs.glsl");

	create_framebuffer(width, height);

	projection = glm::perspective(glm::radians(70.0f), width / height, 0.1f, 100.0f);
	ortProjection = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, 0.1f, 100.0f);
}

void Render::create_framebuffer(float width, float height)
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &colorBufferTexture);
	glBindTexture(GL_TEXTURE_2D, colorBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTexture, 0);

	glGenTextures(1, &idBufferTexture);
	glBindTexture(GL_TEXTURE_2D, idBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, idBufferTexture, 0);

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::resize(float width, float height)
{
	projection = glm::perspective(glm::radians(70.0f), width / height, 0.1f, 100.0f);

	glDeleteFramebuffers(1, &framebuffer);
	create_framebuffer(width, height);
}

void Render::draw(Scene* scene, Camera* camera, Skybox* skybox, bool is_edit_tilemap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
	//draw_skybox(skybox, camera);
	draw_tilemap(scene, camera);
	if (!is_edit_tilemap)
		draw_scene(scene, camera);
	draw_from_framebuffer();
}

void Render::draw_scene(Scene* scene, Camera* camera)
{
	int length = scene->ents_numb;
	for (int i = 0; i < length; ++i)
	{
		Entity* ent = scene->ents[i];
//#ifdef EDITOR
//		if (ent == scene->target)
//			continue ;
//#endif
		Model* mod = ent->mod;
		Light* light = scene->point_lights[0];
		glUseProgram(ent->material.shader_id);
		glBindTexture(GL_TEXTURE_2D, ent->material.texture_id);
		glBindVertexArray(mod->vao);
		glStencilMask(0x00);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, ent->position);
		model = glm::rotate(model, glm::radians(ent->angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(ent->angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(ent->angle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, ent->e_scale);

		glm::mat4 MVP = projection * camera->view * model;

		unsigned int model_loc = glGetUniformLocation(ent->material.shader_id, "u_M");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		unsigned int view_loc = glGetUniformLocation(ent->material.shader_id, "u_V");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera->view));
		unsigned int proj_loc = glGetUniformLocation(ent->material.shader_id, "u_P");
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(ortProjection));

		unsigned int mvp_loc = glGetUniformLocation(ent->material.shader_id, "u_MVP");
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(MVP));

		glUniform1i(glGetUniformLocation(ent->material.shader_id, "lightNumb"), 1);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "lightPos"), light->position.x, light->position.y, light->position.z + 2.0f);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "viewPos"), camera->pos.x, camera->pos.y, camera->pos.z);
		glUniform1i(glGetUniformLocation(ent->material.shader_id, "material.diffuse"), 0);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "material.specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "material.shininess"), 16.0f);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.ambient"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.diffuse"), light->color[0], light->color[1], light->color[2]);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.specular"), 0.4f, 0.4f, 0.4f);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.constant"), light->constant);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.linear"), light->linear);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.quadratic"), light->quadratic);

		glDrawArrays(GL_TRIANGLES, 0, mod->ind_number);
	}
}

void Render::draw_target(Scene* scene, Camera* camera)
{
	Entity* ent = scene->target;
	Model* mod = ent->mod;
	Light* light = scene->point_lights[0];
	glUseProgram(ent->material.shader_id);
	glBindTexture(GL_TEXTURE_2D, ent->material.texture_id);
	glBindVertexArray(mod->vao);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, ent->position);
	model = glm::rotate(model, glm::radians(ent->angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ent->angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ent->angle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, ent->e_scale);

	unsigned int model_loc = glGetUniformLocation(ent->material.shader_id, "u_M");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int view_loc = glGetUniformLocation(ent->material.shader_id, "u_V");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera->view));
	unsigned int proj_loc = glGetUniformLocation(ent->material.shader_id, "u_P");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(ortProjection));

	glUniform1i(glGetUniformLocation(ent->material.shader_id, "lightNumb"), 1);
	glUniform3f(glGetUniformLocation(ent->material.shader_id, "lightPos"), light->position.x, light->position.y, light->position.z + 2.0f);
	glUniform3f(glGetUniformLocation(ent->material.shader_id, "viewPos"), camera->pos.x, camera->pos.y, camera->pos.z);
	glUniform1i(glGetUniformLocation(ent->material.shader_id, "material.diffuse"), 0);
	glUniform3f(glGetUniformLocation(ent->material.shader_id, "material.specular"), 0.2f, 0.2f, 0.2f);
	glUniform1f(glGetUniformLocation(ent->material.shader_id, "material.shininess"), 16.0f);
	glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.ambient"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.diffuse"), light->color[0], light->color[1], light->color[2]);
	glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.specular"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.constant"), light->constant);
	glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.linear"), light->linear);
	glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.quadratic"), light->quadratic);

	glDrawArrays(GL_TRIANGLES, 0, mod->ind_number);

	int target_shader_id = scene->shader_atlas["target"];
	glUseProgram(target_shader_id);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	model = glm::scale(model, glm::vec3(1.1f));

	model_loc = glGetUniformLocation(target_shader_id, "u_M");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	view_loc = glGetUniformLocation(target_shader_id, "u_V");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera->view));
	proj_loc = glGetUniformLocation(target_shader_id, "u_P");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, mod->ind_number);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void	Render::draw_tilemap(Scene* scene, Camera* camera)
{
	Tilemap* tilemap = &scene->tilemap;
	Light* light = scene->point_lights[0];
	Model* mod = &tilemap->mod;

	glDepthMask(GL_FALSE);
	glUseProgram(tilemap->shader_id);
	glBindTexture(GL_TEXTURE_2D, tilemap->texture_id);
	glBindVertexArray(mod->vao);
	glStencilMask(0x00);

	glm::mat4 model = glm::mat4(1.0f);

	unsigned int model_loc = glGetUniformLocation(tilemap->shader_id, "u_M");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int view_loc = glGetUniformLocation(tilemap->shader_id, "u_V");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera->view));
	unsigned int proj_loc = glGetUniformLocation(tilemap->shader_id, "u_P");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(ortProjection));

	glUniform1i(glGetUniformLocation(tilemap->shader_id, "lightNumb"), 1);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "lightPos"), light->position.x, light->position.y, light->position.z + 2.0f);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "viewPos"), camera->pos.x, camera->pos.y, camera->pos.z);
	glUniform1i(glGetUniformLocation(tilemap->shader_id, "material.diffuse"), 0);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "material.specular"), 0.2f, 0.2f, 0.2f);
	glUniform1f(glGetUniformLocation(tilemap->shader_id, "material.shininess"), 16.0f);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "light.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "light.diffuse"), light->color[0], light->color[1], light->color[2]);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "light.specular"), 0.4f, 0.4f, 0.4f);
	glUniform1f(glGetUniformLocation(tilemap->shader_id, "light.constant"), light->constant);
	glUniform1f(glGetUniformLocation(tilemap->shader_id, "light.linear"), light->linear);
	glUniform1f(glGetUniformLocation(tilemap->shader_id, "light.quadratic"), light->quadratic);

	glDrawArrays(GL_TRIANGLES, 0, mod->ind_number);
	glDepthMask(GL_TRUE);
}

void	Render::draw_from_framebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.4f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(screen_shader);
	glBindVertexArray(screenVAO);
	glBindTexture(GL_TEXTURE_2D, colorBufferTexture);	// use the color attachment texture as the texture of the quad plane
	//glBindTexture(GL_TEXTURE_2D, idBufferTexture);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void	Render::draw_skybox(Skybox *skybox, Camera* camera)
{
	glDepthMask(GL_FALSE);
	glUseProgram(skybox->shader_id);
	glm::mat4 view = glm::mat4(glm::mat3(camera->view));
	unsigned int view_loc = glGetUniformLocation(skybox->shader_id, "u_V");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int proj_loc = glGetUniformLocation(skybox->shader_id, "u_P");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(skybox->vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

int		Render::read_pixel(int x, int y)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	int pix = -1;
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pix);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return pix;
}

Render::~Render()
{
	glDeleteFramebuffers(1, &framebuffer);
}