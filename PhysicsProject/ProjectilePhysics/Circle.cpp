#include "Circle.h"
#include "PhysicsObject.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, bool solid)
	: Rigidbody(CIRCLE, position, velocity, 0, mass, solid), m_radius(radius), m_colour(colour)
{
}

Circle::~Circle()
{
}

void Circle::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 40, m_colour);
}

bool Circle::checkCollision(PhysicsObject* pOther)
{
	if (!m_isSolid)
		return false;

	// Dynamically casts the physics object to a circle object
	Circle* pCircle = dynamic_cast<Circle*>(pOther);
	// If successful, check the distance and compare to radii
	if (pCircle != nullptr)
	{
		const float distance = glm::distance(m_position, pCircle->m_position);
		if (distance < m_radius + pCircle->m_radius)
		{
			return true;
		}
	}
	return false;
}