#pragma once

#include "entity.h"

class Light : virtual public Entity
{
public:
	float color[3] = {1.0f, 1.0f, 1.0f};
	float constant;
	float linear;
	float quadratic;
public:
	~Light() {};
	virtual void	update() override;
};