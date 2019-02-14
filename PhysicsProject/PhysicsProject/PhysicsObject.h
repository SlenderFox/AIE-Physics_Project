#pragma once
#include <glm\ext.hpp>

#ifndef SHAPE_COUNT
	#define SHAPE_COUNT 3
#endif

enum ShapeType
{
	PLANE = 0,
	CIRCLE,
	RECTANGLE
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType pShapeID) : m_shapeID(pShapeID) {}

public:
	virtual void fixedUpdate(glm::vec2 pGravity, float pTimeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};
	ShapeType getShapeID() const { return m_shapeID; }

protected:
	ShapeType m_shapeID;		// The type of object
};