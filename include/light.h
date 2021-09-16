#pragma once

#include "entity.h"

class Light : virtual public Entity
{
	~Light() {};
	virtual void	update() override;
};