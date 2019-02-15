#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody();
	Rigidbody(ShapeType pShapeID, glm::vec2 pPosition, glm::vec2 pVelocity,
		float pRotation, float pAngularVelocity, float pMass,
		float pLinearDrag, float pAngularDrag, float pElasticity,
		bool pSolid, bool pWeighted);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 pGravity, float pTimeStep);
	virtual void debug();
	void applyForce(glm::vec2 pForce);
	void applyForceToActor(Rigidbody* pActor2, glm::vec2 pForce);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	void resolveCollision(Rigidbody* pActor2);

	void setPosition(const glm::vec2 pPosition) { m_position = pPosition; }
	glm::vec2 getPosition() const { return m_position; }

	void setRotation(const float pRotation) { m_rotation = pRotation; }
	float getRotation() { return m_rotation; }

	void setVelocity(const glm::vec2 pVelocity) { m_velocity = pVelocity; }
	glm::vec2 getVelocity() const { return m_velocity; }

	void setMass(const float pMass) { m_mass = pMass; }
	float getMass() const { return m_mass; }

	void setLinearDrag(const float pLinearDrag) { m_linearDrag = pLinearDrag; }
	float getLinearDrag() const { return m_linearDrag; }

	void setAngular(const float pAngular) { m_angularDrag = pAngular; }
	float getAngularDrag() const { return m_angularDrag; }

	void setElasticity(const float pElasticity) { m_elasticity = pElasticity; }
	float getElasticity() const { return m_elasticity; }

protected:
	bool			m_isSolid;						// Whether the object can collider
	bool			m_isWeighted;					// Whether the object is affected by gravity

	glm::vec2	m_position;						// Current position of the object
	glm::vec2	m_lastPosition;				// The position of the object in the last frame
	glm::vec2	m_velocity;						// Current velocity of the object
	float			m_angularVelocity;			// The current rotational velocity of the object
	float			m_mass;							// Mass of the object
	float			m_rotation;						// Current rotation of the object
	float			m_linearDrag;					// The drag causing an object to slow down
	float			m_angularDrag;				// The drag causing an object to rotate
	float			m_elasticity;					// The amount of energy lost per collision

	float			m_velocityCheckTimer;		// Timer to prevent an object getting locked in place
	float			m_angularVCheckTimer;		// Timer to prevent an object getting locked in a rotation
};