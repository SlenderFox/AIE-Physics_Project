#pragma once
#include "Rigidbody.h"
#include <glm\vec4.hpp>

class Circle : public Rigidbody
{
public:
	Circle(glm::vec2 position, glm::vec2 velocity,
		float mass, float radius, glm::vec4 colour, bool solid);
	~Circle();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

protected:
	float			m_radius;
	glm::vec4	m_colour;
};