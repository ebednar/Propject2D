#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "events.h"
#include "entity/player.h"
#include "glm/glm.hpp"

#include <iostream>

inline int direction(float n)
{
	if (n >= 0.0f)
		return 1;
	else
		return -1;
}

void	Player::awake()
{
	scale((float)sub_width / sub_height, 1.0f, 1.0f);
}

void	Player::update()
{
	set_state(state::Idle);

	glm::vec3 movement(0.0f);

	if (events->keys[GLFW_KEY_W])
		movement += glm::vec3(0.0f, 0.1f * speed, 0.0f);
	if (events->keys[GLFW_KEY_A])
		movement += glm::vec3(-0.1f * speed, 0.0f, 0.0f);
	if (events->keys[GLFW_KEY_S])
		movement += glm::vec3(0.0f, -0.1f * speed, 0.0f);
	if (events->keys[GLFW_KEY_D])
		movement += glm::vec3(0.1f * speed, 0.0f, 0.0f);

	if (movement.x != 0.0f || movement.y != 0.0f)
		move_player(movement.x, movement.y, movement.z);
}

void	Player::play_animation()
{
	animator.play_animation();
	if (animator.frame_changed())
	{ 
		set_subtexture(animator.get_frame(), 0, sub_width, sub_height);
	}
}

void	Player::set_state(state new_state)
{
	if (new_state == current_state)
		return;
	current_state = new_state;
	animator.set_animation((int)new_state);
	material.set_texture(animator.current_animation->texture_id);
}

void	Player::set_state(int new_state)
{
	if ((state)new_state == current_state)
		return;
	current_state = (state)new_state;
	animator.set_animation(new_state);
	material.set_texture(animator.current_animation->texture_id);
}

void	Player::move_player(float x, float y, float z)
{
	move(x, y, z);
	if (x)
		scale(direction(x) * (float)sub_width / sub_height, 1.0f, 1.0f);
	set_state(state::Walk);
}