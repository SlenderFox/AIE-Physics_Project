#include "Rigidbody.h"

#ifndef MIN_LINEAR_THRESHOLD
	#define MIN_LINEAR_THRESHOLD 0.05f
#endif

#ifndef MIN_ANGULAR_THRESHOLD
	#define MIN_ROTATIONAL_THRESHOLD 0.01f
#endif


Rigidbody::Rigidbody()
	: PhysicsObject(CIRCLE), m_position(0, 0), m_velocity(0, 0),
	m_rotation(0), m_angularVelocity(0),
	m_mass(1.0f), m_moment(1.0f), m_elasticity(1.0f),
	m_linearDrag(0.0f), m_angularDrag(0.0f),
	m_isSolid(true), m_isWeighted(true)
{
	m_velocityCheckTimer = 0;
	m_angularVCheckTimer = 0;
}

Rigidbody::Rigidbody(ShapeType pShapeID, glm::vec2 pPosition, glm::vec2 pVelocity,
	float pRotation, float pAngularVelocity,
	float pMass, float pMoment, float pElasticity,
	float pLinearDrag, float pAngularDrag,
	bool pSolid, bool pWeighted)
	: PhysicsObject(pShapeID), m_position(pPosition), m_velocity(pVelocity), 
	m_rotation(pRotation), m_angularVelocity(pAngularVelocity),
	m_mass(pMass), m_moment(pMoment), m_elasticity(pElasticity),
	m_linearDrag(pLinearDrag), m_angularDrag(pAngularDrag),
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

	// Applies
	m_velocity -= m_velocity * m_linearDrag * pTimeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * pTimeStep;

	m_position += m_velocity * pTimeStep;
	m_rotation += m_angularVelocity * pTimeStep;
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 pForce)
{
	// Pushes the object
	m_velocity += pForce / m_mass;
}

void Rigidbody::applyForce(glm::vec2 pForce, glm::vec2 pPos)
{
	// Pushes the object with rotation
	m_velocity += pForce / m_mass;
	m_angularVelocity += (pForce.y * pPos.x - pForce.x * pPos.y) / (m_moment);
}

//void Rigidbody::applyForceToActor(Rigidbody* pOtherActor, glm::vec2 pForce, glm::vec2 pPos)
//{
//	// Newton's third law ~ For every action there is an equal and opposite reaction
//	applyForce(pForce);
//	pOtherActor->applyForce(-pForce);
//}

void Rigidbody::resolveCollision(Rigidbody* pActor2, glm::vec2 pContact, glm::vec2* collisionNormal)
{
	// Find the collision normal
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : pActor2->getPosition() - m_position);
	// Find the angle perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// Determine the total velocity of the contact points for the two objects
	// for both linear and rotational

	// 'r' is the radius from axis to the application force
	float r1 = glm::dot(pContact - m_position, -perp);
	float r2 = glm::dot(pContact - pActor2->m_position, perp);
	// Velocity of the contact point for each object
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(pActor2->m_velocity, normal) + r2 * pActor2->m_angularVelocity;

	if (v1 > v2)
	{
		// Calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied
		float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_moment);
		float mass2 = 1.0f / (1.0f / pActor2->m_mass + (r2*r2) / pActor2->m_moment);

		float elasticity = (m_elasticity + pActor2->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity)*mass1*mass2
			/ (mass1 + mass2)*(v1 - v2)*normal;

		applyForce(-force, pContact - m_position);
		pActor2->applyForce(force, pContact - pActor2->m_position);
	}
}