#pragma once

#include "entity/entity.h"

struct Input;

class Player : virtual public Entity
{
public:
	Input*	game_input = nullptr;
public:
	Player() {};
	~Player() {};

	// override entity
	virtual void	awake() override;
	virtual void	update() override;
	virtual void	play_animation() override;
	virtual	void	set_state(int new_state) override;
	// end override entity
private:
	void	set_state(state new_state);
	void	move_player(float x, float y, float z);
private:
	state	current_state = state::Idle;
	float	speed = 0.07f;
};