#include "Plane.h"
#include <Gizmos.h>
#include "Rigidbody.h"

Plane::Plane()
	: PhysicsObject(PLANE), m_normal(0, 1), m_distanceToOrigin(0), m_colour(1, 1 ,1 ,1)
{
}

Plane::Plane(glm::vec2 pNormal, float pDistance, glm::vec4 pColour)
	: PhysicsObject(PLANE), m_normal(pNormal), m_distanceToOrigin(pDistance), m_colour(pColour)
{
}

Plane::~Plane()
{
}

void Plane::fixedUpdate(glm::vec2 pGravity, float pTimeStep)
{
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// Easy to rotate normal 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	// Determines start and end points for the drawing
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, m_colour);
}

void Plane::resetPosition()
{
}

void Plane::resolveCollision(Rigidbody* pActor2, glm::vec2 pContact)
{
	// The plane isn't moving, so the relative velocity is just the actors velocity
	glm::vec2 vRel = pActor2->getVelocity();
	float e = pActor2->getElasticity();
	float j = glm::dot(-(1 + e) * (vRel), m_normal) / (1 / pActor2->getMass());

	glm::vec2 force(m_normal * j);

	pActor2->applyForce(force, pContact - pActor2->getPosition());
}