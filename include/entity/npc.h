#pragma once

#include "entity/entity.h"

class Npc : virtual public Entity
{
public:

public:
	Npc() {};
	~Npc() {};

	// override entity
	virtual void	awake() override;
	virtual void	update() override;
	virtual void	play_animation() override;
	virtual	void	set_state(int new_state) override;
	// end override entity
private:
	void	set_state(state new_state);
private:
	state	current_state = state::Idle;
	float	speed = 0.7f;
};