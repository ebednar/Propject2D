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
	glm::vec3	position;
	glm::vec3	e_scale;
	glm::vec3	angle;
	entity_type	type;
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
