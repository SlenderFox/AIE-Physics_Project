#include "Rigidbody.h"

#ifndef MIN_LINEAR_THRESHOLD
	#define MIN_LINEAR_THRESHOLD 0.05f
#endif

#ifndef MIN_ANGULAR_THRESHOLD
	#define MIN_ROTATIONAL_THRESHOLD 0.01f
#endif


Rigidbody::Rigidbody()
	: PhysicsObject(CIRCLE), m_position(0, 0), m_velocity(0, 0),
	m_rotation(0), m_angularVelocity(0), m_mass(1.0f),
	m_linearDrag(0.0f), m_angularDrag(0.0f), m_elasticity(1.0f),
	m_isSolid(true), m_isWeighted(true)
{
	m_velocityCheckTimer = 0;
	m_angularVCheckTimer = 0;
}

Rigidbody::Rigidbody(ShapeType pShapeID, glm::vec2 pPosition, glm::vec2 pVelocity,
	float pRotation, float pAngularVelocity, float pMass,
	float pLinearDrag, float pAngularDrag, float pElasticity,
	bool pSolid, bool pWeighted)
	: PhysicsObject(pShapeID), m_position(pPosition), m_velocity(pVelocity), 
	m_rotation(pRotation), m_angularVelocity(pAngularVelocity), m_mass(pMass),
	m_linearDrag(pLinearDrag), m_angularDrag(pAngularDrag), m_elasticity(pElasticity),
	m_isSolid(pSolid), m_isWeighted(pWeighted)
{
	m_velocityCheckTimer = 0;
	m_angularVCheckTimer = 0;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 pGravity, float pTimeStep)
{
	if (m_isWeighted)
		applyForce(pGravity * m_mass * pTimeStep);

	if (m_velocityCheckTimer < 0.5f)
		m_velocityCheckTimer += pTimeStep;

	if (m_angularVCheckTimer < 0.5f)
		m_angularVCheckTimer += pTimeStep;

	if (m_velocityCheckTimer >= 0.5f && length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
		m_velocityCheckTimer = 0;
	}

	if (m_angularVCheckTimer >= 0.5f && abs(m_angularVelocity) < MIN_ROTATIONAL_THRESHOLD)
	{
		m_angularVelocity = 0;
		m_angularVCheckTimer = 0;
	}

	m_velocity -= m_velocity * m_linearDrag * pTimeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * pTimeStep;

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

	float elasticity = (m_elasticity + pActor2->getElasticity()) / 2.0f;

	// Calculate the impulse magnitude
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		glm::dot(normal, normal * ((1 / m_mass) + (1 / pActor2->getMass())));

	glm::vec2 force = normal * j;

	applyForceToActor(pActor2, -force);
}