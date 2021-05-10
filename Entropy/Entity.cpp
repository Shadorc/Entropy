#include "Circle.h"
#include "Rectangle.h"
#include "Component.h"
#include "Entity.h"

static int id = 0;

Entity::Entity(float x, float y) :
	m_id(id++),
	m_position(new Vector2(x, y)),
	m_velocity(new Vector2(0, 0))
{

}

Entity::~Entity()
{
	delete m_position;
	delete m_velocity;
	for (Component* component : m_components)
	{
		delete component;
	}
	m_components.clear();
}

void Entity::AddComponent(Component* component)
{
	m_components.push_back(component);
}

Vector2* Entity::GetPosition() const
{
	return m_position;
}

Vector2* Entity::GetVelocity() const
{
	return m_velocity;
}

void Entity::Update(float delta)
{
	for (Component* component : m_components)
	{
		component->Update(delta);
	}
}

bool Entity::Intersects(const Entity* other) const
{
	const entity::Rectangle* rectangle = dynamic_cast<const entity:: Rectangle* > (other);
	if (rectangle != nullptr) {
		return Intersects(rectangle);
	}

	const entity::Circle* circle = dynamic_cast<const entity::Circle*>(other);
	if (circle != nullptr) {
		return Intersects(circle);
	}

	/* TODO
	Line* line = dynamic_cast<Line*>(other);
	if (line !m= nullptr) {
		return Intersects(line);
	}
	*/

	return false;
}

Vector2 Entity::ComputePenetrationVector(const Entity* other) const
{
	const entity::Rectangle* rectangle = dynamic_cast<const entity::Rectangle*>(other);
	if (rectangle != nullptr)
	{
		return ComputePenetrationVector(rectangle);
	}
	const entity::Circle* circle = dynamic_cast<const entity::Circle*>(other);
	if (circle != nullptr)
	{
		return ComputePenetrationVector(circle);
	}
	return Vector2();
}

bool Entity::operator==(Entity& other) const
{
	return m_id == other.m_id;
}
