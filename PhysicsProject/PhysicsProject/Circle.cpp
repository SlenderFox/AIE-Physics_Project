#include "Circle.h"
#include "PhysicsObject.h"
#include <Gizmos.h>

Circle::Circle()
	: Rigidbody(CIRCLE, glm::vec2(0, 0), glm::vec2(0, 0), 0, 1.0f, 0.6f, 0.6f, true, true), m_radius(1.0f), m_colour(glm::vec4(1, 1, 1, 1))
{
}

Circle::Circle(glm::vec2 pPosition, glm::vec2 pVelocity, float pMass,
	float pRadius, glm::vec4 pColour, bool pSolid, bool pWeighted)
	: Rigidbody(CIRCLE, pPosition, pVelocity,0, pMass, pSolid, pWeighted),
	m_radius(pRadius), m_colour(pColour)
{
}

Circle::~Circle()
{
}

void Circle::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
}

bool Circle::checkCollision(PhysicsObject* pOther)
{
	//if (!m_isSolid)
	//	return false;

	//// Dynamically casts the physics object to a circle object
	//Circle* pCircle = dynamic_cast<Circle*>(pOther);
	//// If successful, check the distance and compare to radii
	//if (pCircle != nullptr)
	//{
	//	const float distance = glm::distance(m_position, pCircle->m_position);
	//	if (distance < m_radius + pCircle->m_radius)
	//	{
	//		return true;
	//	}
	//}
	return false;
}