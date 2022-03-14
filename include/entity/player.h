#pragma once

#include "entity/entity.h"

enum class state
{
	Idle = 0,
	Walk,
	Attack
};

class Player : virtual public Entity
{
public:
	
public:
	Player();
	~Player() {};

	// override entity
	virtual void	awake() override;
	virtual void	update() override;
	virtual void	play_animation() override;
private:
	state player_state = state::Idle;
};