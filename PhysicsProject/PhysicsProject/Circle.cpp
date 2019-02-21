#include "Circle.h"
#include "PhysicsObject.h"
#include <Gizmos.h>

Circle::Circle()
	: Rigidbody(CIRCLE, glm::vec2(0, 0), glm::vec2(0, 0),
		1.0f, 0.5f, 1.0f,
		0, 0,
		0.3f, 0.3f,
		true, true),
	m_radius(1.0f), m_colour(glm::vec4(1, 1, 1, 1))
{
}

Circle::Circle(glm::vec2 pPosition, glm::vec2 pVelocity,
	float pRotation, float pAngularVelocity,
	float pMass, float pElasticity,
	float pLinearDrag, float pAngularDrag,
	bool pSolid, bool pWeighted,
	float pRadius, glm::vec4 pColour)
	: Rigidbody(CIRCLE, pPosition, pVelocity,
		pRotation, pAngularVelocity,
		pMass, (0.5f * pMass * pRadius * pRadius), pElasticity,
		pLinearDrag, pAngularDrag,
		pSolid, pWeighted),
	m_radius(pRadius), m_colour(pColour)
{
}

Circle::~Circle()
{
}

void Circle::makeGizmo()
{
	glm::vec2 end = glm::vec2(std::cosf(m_rotation), std::sinf(m_rotation)) * m_radius;
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}