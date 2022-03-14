#pragma once
#include "model.h"
#include "glm/glm.hpp"
#include "material.h"
#include "animator.h"

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
	Animator	animator;
	glm::vec3	position = {0.0f, 0.0f, 0.0f};
	glm::vec3	e_scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3	angle = { 0.0f, 0.0f, 0.0f };
	glm::mat4	MVP;
	entity_type	type;
	int			id;
	int			sub_width = 512;
	int			sub_height = 512;
public:
	Entity();
	virtual ~Entity();
	virtual void	set_model(Model *mod);
	virtual void	move(float x, float y, float z);
	virtual void	move_to(float x, float y, float z);
	virtual void	rotate(float x, float y, float z);
	virtual void	rotate_to(float x, float y, float z);
	virtual void	scale(float x, float y, float z);
	virtual void	awake() {};
	virtual void	update() {};
	virtual void	play_animation() {};
	void			init_animations();
	virtual void	set_subtexture(int xpad = 0, int ypad = 0, int width = 512, int height = 512);
private:
};
