#pragma once
#include <glm\vec2.hpp>
#include <vector>

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void update(float pDeltaTime);
	void updateGizmos();
	void debugScene();

	void addActor(PhysicsObject* pActor);
	bool removeActor(PhysicsObject* pActor);

	PhysicsObject* getActor(const int pPos) const;
	std::vector<PhysicsObject*> getActors() const;

	void setGravity(const glm::vec2 pGravity) { m_gravity = pGravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float pTimeStep) { m_timeStep = pTimeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollision();

	// Planes
	static bool planeToPlane(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool planeToCircle(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool planeToRectangle(PhysicsObject* pObject1, PhysicsObject* pObject2);
	// Circles
	static bool circleToPlane(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool circleToCircle(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool circleToRectangle(PhysicsObject* pObject1, PhysicsObject* pObject2);
	// Rectangles
	static bool RectangleToPlane(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool RectangleToCircle(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool RectangleToRectangle(PhysicsObject* pObject1, PhysicsObject* pObject2);

protected:
	glm::vec2							m_gravity;		// The gravity of the simulation
	float									m_timeStep;	// The regular intervals at which calculations are processed
	std::vector<PhysicsObject*> m_actors;		// All objects under influence of the physics scene
};