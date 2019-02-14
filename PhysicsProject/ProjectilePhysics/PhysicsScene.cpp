#include "PhysicsScene.h"
#include <iostream>
#include <list>
#include "PhysicsObject.h"
#include "Rigidbody.h"

using std::cout;

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(0, 0)
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;

	// Update physics at a fixed time step

	static float  accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		for (auto pActor : m_actors)
		{
			Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
			if (pRigid->getPosition().x < -110 || pRigid->getPosition().x > 110 || pRigid->getPosition().y < -70 || pRigid->getPosition().y > 70)
			{
				removeActor(pActor);
			}
		}

		// Check for collisions
		for (auto pActor : m_actors)
		{
			for (auto pOther : m_actors)
			{
				if (pActor == pOther)
					continue;
				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				if (pRigid->checkCollision(pOther) == true)
				{
					pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther),
											pRigid->getVelocity() * pRigid->getMass());
					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}
			}
		}
		dirty.clear();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

bool PhysicsScene::removeActor(PhysicsObject* actor)
{
	for (int i = 0; i < m_actors.size(); ++i)
	{
		if (m_actors[i] == actor)
		{
			m_actors.erase(m_actors.begin() + i);
			return true;
		}
	}
	return false;
}

PhysicsObject* PhysicsScene::getActor(const int pPos) const
{
	return m_actors[pPos];
}

std::vector<PhysicsObject*> PhysicsScene::getActors() const
{
	return m_actors;
}
