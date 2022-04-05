#pragma once

#include "entity/entity.h"

class Ui_text;

class Npc : virtual public Entity
{
public:
	Ui_text* texter;
public:
	Npc() {};
	~Npc() {};

	// override entity
	virtual void	awake() override;
	virtual void	update() override;
	virtual void	play_animation() override;
	virtual	void	set_state(int new_state) override;
	virtual void	interact() override;
	// end override entity
private:
	void	set_state(state new_state);
private:
	state	current_state = state::Idle;
	float	speed = 0.7f;
};