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
	static bool planeToPlane(PhysicsObject* pPlane1, PhysicsObject* pPlane2);
	static bool planeToCircle(PhysicsObject* pPlane, PhysicsObject* pCircle);
	static bool planeToAABB(PhysicsObject* pPlane, PhysicsObject* pAABB);
	// Circles
	static bool circleToPlane(PhysicsObject* pCircle, PhysicsObject* pPlane);
	static bool circleToCircle(PhysicsObject* pCircle1, PhysicsObject* pCircle2);
	static bool circleToAABB(PhysicsObject* pCircle, PhysicsObject* pAABB);
	// AABBs
	static bool AABBToPlane(PhysicsObject* pAABB, PhysicsObject* pPlane);
	static bool AABBToCircle(PhysicsObject* pAABB, PhysicsObject* pCircle);
	static bool AABBToAABB(PhysicsObject* pAABB1, PhysicsObject* pAABB2);

protected:
	glm::vec2							m_gravity;		// The gravity of the simulation
	float									m_timeStep;	// The regular intervals at which calculations are processed
	std::vector<PhysicsObject*> m_actors;		// All objects under influence of the physics scene
};