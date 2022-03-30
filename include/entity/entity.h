#pragma once
#include "model.h"
#include "glm/glm.hpp"
#include "material.h"
#include "animator.h"

enum class entity_type
{
	Player,
	Npc,
	Obstacle,
	Light
};

enum class state
{
	Idle = 0,
	Walk,
	Attack
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
	float		distance = 10.0f;
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
	void			update_dist(Entity* player);
	virtual void	interact() {};
	virtual void	play_animation() {};
	virtual void	set_subtexture(int xpad = 0, int ypad = 0, int width = 512, int height = 512);
	virtual	void	set_state(int new_state) {};
	void			init_animations();
private:
};
