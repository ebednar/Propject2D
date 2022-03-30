#include "entity/entity.h"
#include "glm/glm.hpp"

#include <math.h>

Entity::Entity()
{
	move_to(0.0f, 0.0f, 0.0f);
	angle[0] = 0.0f;
	angle[1] = 0.0f;
	angle[2] = 0.0f;
	scale(1.0f, 1.0f, 1.0f);
}

Entity::~Entity()
{
	delete[] mod->vertices;
	delete mod;
}

void Entity::set_model(Model *model)
{
	if (model == NULL)
		exit ;
	mod = new Model();
	mod->vertices = new Vertex[model->ind_number];
	mod->ind_number = model->ind_number;
	for (int i = 0; i < mod->ind_number; ++i)
	{
		mod->vertices[i].pos[0] = model->vertices[i].pos[0];
		mod->vertices[i].pos[1] = model->vertices[i].pos[1];
		mod->vertices[i].pos[2] = model->vertices[i].pos[2];
		mod->vertices[i].uv[0] = model->vertices[i].uv[0];
		mod->vertices[i].uv[1] = model->vertices[i].uv[1];
		mod->vertices[i].normal[0] = model->vertices[i].normal[0];
		mod->vertices[i].normal[1] = model->vertices[i].normal[1];
		mod->vertices[i].normal[2] = model->vertices[i].normal[2];
	}
	mod->vertex_buffer();
}

void Entity::move(float x, float y, float z)
{
	position[0] += x;
	position[1] += y;
	position[2] += z;
}

void Entity::move_to(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void Entity::rotate(float x, float y, float z)
{
	angle[0] += x;
	angle[1] += y;
	angle[2] += z;
}

void Entity::rotate_to(float x, float y, float z)
{
	angle[0] = x;
	angle[1] = y;
	angle[2] = z;
}

void Entity::scale(float x, float y, float z)
{
	e_scale[0] = x;
	e_scale[1] = y;
	e_scale[2] = z;
}

void	Entity::set_subtexture(int xpad, int ypad, int width, int height)
{
	mod->vertices[0].uv[0] = (xpad + 1) * (float)width / material.text_width;
	mod->vertices[0].uv[1] = (ypad + 1) * (float)height / material.text_height;
	mod->vertices[1].uv[0] = xpad * (float)width / material.text_width;
	mod->vertices[1].uv[1] = (ypad + 1) * (float)height / material.text_height;
	mod->vertices[2].uv[0] = xpad * (float)width / material.text_width;
	mod->vertices[2].uv[1] = (ypad + 0.1) * (float)height / material.text_width;
	mod->vertices[3].uv[0] = xpad * (float)width / material.text_width;
	mod->vertices[3].uv[1] = (ypad + 0.1) * (float)height / material.text_width;
	mod->vertices[4].uv[0] = (xpad + 1) * (float)width / material.text_width;
	mod->vertices[4].uv[1] = (ypad + 0.1) * (float)height / material.text_width;
	mod->vertices[5].uv[0] = (xpad + 1) * (float)width / material.text_width;
	mod->vertices[5].uv[1] = (ypad + 1) * (float)height / material.text_height;
	mod->update_vbo();
}

void	Entity::init_animations()
{
	animator.init(material.text_width, material.text_height);
}

void	Entity::update_dist(Entity* player)
{
	distance = fabs(sqrt(pow(player->position.x - position.x, 2.0) + pow(player->position.y - position.y, 2.0)));
}