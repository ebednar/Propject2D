#include "render.h"
#include "glad.h"
#include <string>
#include <iostream>
#include "glm/glm.hpp"

void Render::init(float width, float height)
{
	projection = glm::perspective(glm::radians(70.0f), width / height, 0.1f, 100.0f);
	ortProjection = glm::ortho(0.0f, width, 0.0f, height, 0.1f, 100.0f);
}

void Render::recalc_proj(float width, float height)
{
	projection = glm::perspective(glm::radians(70.0f), width / height, 0.1f, 100.0f);
}

void Render::draw_scene(Scene* scene, Camera* cam)
{
	int length = scene->ents_numb;
	for (int i = 0; i < length; ++i)
	{
		Entity* ent = scene->ents[i];
		Model* mod = ent->mod;
		Light* light = scene->point_lights[0];
		glUseProgram(ent->material.shader_id);
		glBindTexture(GL_TEXTURE_2D, ent->material.texture_id);
		glBindVertexArray(mod->vao);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, ent->position);
		model = glm::rotate(model, glm::radians(ent->angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(ent->angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(ent->angle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, ent->e_scale);

		unsigned int model_loc = glGetUniformLocation(ent->material.shader_id, "u_M");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		unsigned int view_loc = glGetUniformLocation(ent->material.shader_id, "u_V");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(cam->view));
		unsigned int proj_loc = glGetUniformLocation(ent->material.shader_id, "u_P");
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform1i(glGetUniformLocation(ent->material.shader_id, "lightNumb"), 1);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "lightPos"), scene->point_lights[0]->position.x, scene->point_lights[0]->position.y, scene->point_lights[0]->position.z);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "viewPos"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform1i(glGetUniformLocation(ent->material.shader_id, "material.diffuse"), 0);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "material.shininess"), 16.0f);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.diffuse"), light->color[0], light->color[1], light->color[2]);
		glUniform3f(glGetUniformLocation(ent->material.shader_id, "light.specular"), 0.4f, 0.4f, 0.4f);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.constant"), light->constant);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.linear"), light->linear);
		glUniform1f(glGetUniformLocation(ent->material.shader_id, "light.quadratic"), light->quadratic);

		glDrawArrays(GL_TRIANGLES, 0, mod->ind_number);
	}
}

void	Render::draw_tilemap(Scene* scene, Camera* cam)
{
	Tilemap* tilemap = &scene->tilemap;
	Light* light = scene->point_lights[0];
	Model* mod = tilemap->mod;

	glDepthMask(GL_FALSE);
	glUseProgram(tilemap->shader_id);
	glBindTexture(GL_TEXTURE_2D, tilemap->texture_id);
	glBindVertexArray(mod->vao);

	glm::mat4 model = glm::mat4(1.0f);

	unsigned int model_loc = glGetUniformLocation(tilemap->shader_id, "u_M");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int view_loc = glGetUniformLocation(tilemap->shader_id, "u_V");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(cam->view));
	unsigned int proj_loc = glGetUniformLocation(tilemap->shader_id, "u_P");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform1i(glGetUniformLocation(tilemap->shader_id, "lightNumb"), 1);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "lightPos"), scene->point_lights[0]->position.x, scene->point_lights[0]->position.y, scene->point_lights[0]->position.z);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "viewPos"), cam->pos.x, cam->pos.y, cam->pos.z);
	glUniform1i(glGetUniformLocation(tilemap->shader_id, "material.diffuse"), 0);
	glUniform3f(glGetUniformLocation(tilemap->shader_id, "material.specular"), 0.5f, 0.5f, 0.5f);
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

void	Render::draw_skybox(Skybox *skybox, Camera* cam)
{
	glDepthMask(GL_FALSE);
	glUseProgram(skybox->shader_id);
	glm::mat4 view = glm::mat4(glm::mat3(cam->view));
	unsigned int view_loc = glGetUniformLocation(skybox->shader_id, "u_V");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int proj_loc = glGetUniformLocation(skybox->shader_id, "u_P");
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(skybox->vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}
