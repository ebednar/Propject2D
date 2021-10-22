#pragma once

#include "entity.h"

class Light : virtual public Entity
{
public:
	float color[3] = {1.0f, 1.0f, 1.0f};
	float constant = 1.0f;
	float linear = 0.045f;
	float quadratic = 0.0075f;
public:
	~Light() {};
	virtual void	update() override;
};