#pragma once
#include "Rigidbody.h"

class Circle : public Rigidbody
{
public:
	Circle();
	Circle(glm::vec2 pPosition, glm::vec2 pVelocity,
		float pRotation, float pAngularVelocity, float pMass,
		float pLinearDrag, float pAngularDrag,
		bool pSolid, bool pWeighted,
		float pRadius, glm::vec4 pColour);
	~Circle();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	float getRadius() const { return m_radius; }
	glm::vec4 getColour() const { return m_colour; }
	bool getSolid() const { return m_isSolid; }
	bool getWeighted() const { return m_isWeighted; }

protected:
	float			m_radius;	// The radius of the circle
	glm::vec4	m_colour;	// The colour of the circle
};