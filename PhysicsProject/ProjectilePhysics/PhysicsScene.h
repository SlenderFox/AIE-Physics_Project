#pragma once
#include <glm\vec2.hpp>
#include <vector>

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void update(float dt);
	void updateGizmos();
	void debugScene();

	void addActor(PhysicsObject* actor);
	bool removeActor(PhysicsObject* actor);

	PhysicsObject* getActor(const int pPos) const;
	std::vector<PhysicsObject*> getActors() const;

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

protected:
	glm::vec2							m_gravity;
	float									m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};