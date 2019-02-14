#pragma once
#include "PhysicsObject.h"

class Rigidbody;

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 pNormal, float pDistance, glm::vec4 pColour);
	~Plane();

	virtual void fixedUpdate(glm::vec2 pGravity, float pTimeStep);
	virtual void debug() {}
	virtual void makeGizmo();
	virtual void resetPosition();

	void resolveCollision(Rigidbody* pActor2);

	glm::vec2 getNormal() const { return m_normal; }
	float getDistance() const { return m_distanceToOrigin; }

protected:
	glm::vec2	m_normal;					// The perpendicular direction of the plane facing to the origin
	float			m_distanceToOrigin;		// The shortest distance from the origin to the plane
	glm::vec4	m_colour;					// The colour of the plane
};