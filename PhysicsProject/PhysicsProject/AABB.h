#pragma once
#include "Rigidbody.h"

class AABB : public Rigidbody
{
public:
	AABB();
	AABB(glm::vec2 pPosition, glm::vec2 pVelocity,
		float pMass, float pElasticity,
		float pLinearDrag,
		bool pSolid, bool pWeighted,
		float pWidth, float pHeight, glm::vec4 pColour);
	~AABB();

	virtual void makeGizmo();

	void setColour(const glm::vec4 pColour) { m_colour = pColour; }
	glm::vec4 getColour() const { return m_colour; }

	void setWidth(const float pWidth) { m_width = pWidth; }
	float getWidth() const { return m_width; }

	void setHeight(const float pHeight) { m_height = pHeight; }
	float getHeight() const { return m_height; }

	void setSolid(const float pSolid) { m_isSolid = pSolid; }
	bool getSolid() const { return m_isSolid; }

	void setWeighted(const float pWeighted) { m_isWeighted = pWeighted; }
	bool getWeighted() const { return m_isWeighted; }

protected:
	glm::vec4	m_colour;	// The colour of the AABB
	float			m_width;
	float			m_height;
};