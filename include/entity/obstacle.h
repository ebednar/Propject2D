#pragma once
#include "entity.h"

class Obstacle : virtual public Entity
{
	~Obstacle() {};
	virtual void	update() override;
};