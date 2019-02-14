#include "Rigidbody.h"

#ifndef MIN_LINEAR_THRESHOLD
	#define MIN_LINEAR_THRESHOLD
#endif

#ifndef MIN_ANGULAR_THRESHOLD
	#define MIN_ANGULAR_THRESHOLD
#endif


Rigidbody::Rigidbody(ShapeType pShapeID, glm::vec2 pPosition, glm::vec2 pVelocity,
	float pRotation, float pMass, float pLinearDrag, float pAngularDrag, bool pSolid, bool pWeighted)
	: PhysicsObject(pShapeID), m_position(pPosition), m_velocity(pVelocity), 
	m_rotation(pRotation), m_mass(pMass), m_isSolid(pSolid), m_isWeighted(pWeighted)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 pGravity, float pTimeStep)
{
	if (m_isWeighted)
		applyForce(pGravity * m_mass * pTimeStep);

	if (length(m_velocity) < )
	m_velocity -= m_velocity * m_linearDrag * pTimeStep;
	m_angularDrag -= m_angularDrag * m_angularDrag * pTimeStep;

	m_position += m_velocity * pTimeStep;
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 pForce)
{
	// Pushes the object
	m_velocity += pForce / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody* pOtherActor, glm::vec2 pForce)
{
	// Newton's third law ~ For every action there is an equal and opposite reaction
	applyForce(pForce);
	pOtherActor->applyForce(-pForce);
}

void Rigidbody::resolveCollision(Rigidbody* pActor2)
{
	glm::vec2 normal = glm::normalize(pActor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = pActor2->getVelocity() - m_velocity;

	float elasticity = 1;

	// Calculate the impulse magnitude
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		glm::dot(normal, normal * ((1 / m_mass) + (1 / pActor2->getMass())));

	glm::vec2 force = normal * j;

	applyForceToActor(pActor2, -force);
}