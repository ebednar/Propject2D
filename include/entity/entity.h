#pragma once
#include "model.h"
#include "glm/glm.hpp"
#include "material.h"

enum class entity_type
{
	Player,
	Obstacle,
	Light
};

class Entity
{
public:
	Model		*mod;
	Material	material;
	glm::vec3	position = {0.0f, 0.0f, 0.0f};
	glm::vec3	e_scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3	angle = { 0.0f, 0.0f, 0.0f };
	glm::mat4	MVP;
	entity_type	type;
	int			id;
public:
	Entity();
	virtual ~Entity() {};
	virtual void	set_model(Model *mod);
	virtual void	move(float x, float y, float z);
	virtual void	move_to(float x, float y, float z);
	virtual void	rotate(float x, float y, float z);
	virtual void	rotate_to(float x, float y, float z);
	virtual void	scale(float x, float y, float z);
	virtual void	update();
private:
};
