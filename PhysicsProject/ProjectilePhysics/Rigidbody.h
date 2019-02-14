#pragma once
#include "PhysicsObject.h"
#include <glm\vec2.hpp>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position,
			glm::vec2 velocity, float rotation, float mass, bool solid);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(Rigidbody* actor2, glm::vec2 force);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void setMass(float pMass) { m_mass = pMass; }

protected:
	glm::vec2	m_position;
	glm::vec2	m_velocity;
	float			m_mass;
	float			m_rotation; //2D so we only need a single float to represent our rotation
	bool			m_isSolid;
};