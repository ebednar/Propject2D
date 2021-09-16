#pragma once

#include "entity.h"

class Player : virtual public Entity
{
	~Player() {};
	virtual void	update() override;
};