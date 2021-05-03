#pragma once

#include "Entity.h"

class Circle : public Entity
{
private:
	float m_radius;

public:
	Circle(float x, float y, float radius);

	float GetRadius() const;

	void Paint() const;
};
