#include "AABB.h"
#include "PhysicsObject.h"
#include <Gizmos.h>

AABB::AABB()
	: Rigidbody(st_AABB, glm::vec2(0, 0), glm::vec2(0, 0),
		0, 0,
		1.0f, 0.08f, 1.0f,
		0.3f, 0.3f,
		true, true), m_colour(glm::vec4(1, 1, 1, 1)), m_width(1.0f), m_height(1.0f)
{
}

AABB::AABB(glm::vec2 pPosition, glm::vec2 pVelocity,
	float pMass, float pElasticity,
	float pLinearDrag,
	bool pSolid, bool pWeighted,
	float pWidth, float pHeight, glm::vec4 pColour)
	: Rigidbody(st_AABB, pPosition, pVelocity,
		0, 0,
		pMass, (1.0f / 12.0f * pMass * pWidth * pMass), pElasticity,
		pLinearDrag, 0,
		pSolid, pWeighted), m_colour(pColour), m_width(pWidth), m_height(pHeight)
{
}

AABB::~AABB()
{
}

void AABB::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, glm::vec2(m_width, m_height), m_colour);
}