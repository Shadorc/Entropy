#pragma once

#include "Entity.h"

class Circle : public Entity
{
private:
	float m_radius;

public:
	Circle(float x, float y, float radius);

	void Paint() const;
	AABB GetAABB() const override;
};
