#include "PhysicsScene.h"
#include <iostream>
#include <list>
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"

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

void PhysicsScene::update(float pDeltaTime)
{
	//static std::list<PhysicsObject*> dirty;

	// Update physics at a fixed time step

	static float  accumulatedTime = 0.0f;
	accumulatedTime += pDeltaTime;

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

			if (pRigid == nullptr)
				continue;

			if (pRigid->getPosition().x < -110 || pRigid->getPosition().x > 110 || pRigid->getPosition().y < -70 || pRigid->getPosition().y > 70)
				removeActor(pActor);
		}

		checkForCollision();
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
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::addActor(PhysicsObject* pActor)
{
	m_actors.push_back(pActor);
}

bool PhysicsScene::removeActor(PhysicsObject* pActor)
{
	for (int i = 0; i < m_actors.size(); ++i)
	{
		if (m_actors[i] == pActor)
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

// Function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::planeToPlane, PhysicsScene::planeToCircle, PhysicsScene::planeToAABB,
	PhysicsScene::circleToPlane, PhysicsScene::circleToCircle, PhysicsScene::circleToAABB,
	PhysicsScene::AABBToPlane, PhysicsScene::AABBToCircle, PhysicsScene::AABBToAABB,
};

void PhysicsScene::checkForCollision()
{
	int actorCount = (int)m_actors.size();

	// Need to check collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeID1 = object1->getShapeID();
			int shapeID2 = object2->getShapeID();

			// Using function pointers
			int functionIdx = (shapeID1 * SHAPE_COUNT) + shapeID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			// Did a collision occur?
			if (collisionFunctionPtr != nullptr)
				collisionFunctionPtr(object1, object2);
		}
	}
}

bool PhysicsScene::planeToPlane(PhysicsObject* pPlane1, PhysicsObject* pPlane2)
{
	return false;
}

bool PhysicsScene::planeToCircle(PhysicsObject* pPlane, PhysicsObject* pCircle)
{
	// Dynamically casts the physics objects to circle objects
	Plane* plane = dynamic_cast<Plane*>(pPlane);
	Circle* circle = dynamic_cast<Circle*>(pCircle);

	if (!(plane == nullptr || circle == nullptr))
	{
		// If one of the objects isnt solid return
		if (!circle->getSolid())
			return false;

		// Projects the circles distance from the origin onto the planes normal
		glm::vec2 collisionNormal = plane->getNormal();
		float circleToPlane = glm::dot(circle->getPosition(), plane->getNormal()) - plane->getDistance();

		// If we are behind plane then we flip the normal
		if (circleToPlane < 0)
		{
			collisionNormal *= -1;
			circleToPlane *= -1;
		}

		float intersection = circle->getRadius() - circleToPlane;
		if (intersection > 0)
		{
			// Resitution
			circle->setPosition(circle->getPosition() + collisionNormal * intersection);

			glm::vec2 contact = circle->getPosition() + (collisionNormal * -circle->getRadius());

			plane->resolveCollision(circle, contact);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::planeToAABB(PhysicsObject* pPlane, PhysicsObject* pAABB)
{
	return false;
}

// Redundant, call counterpart
bool PhysicsScene::circleToPlane(PhysicsObject* pCircle, PhysicsObject* pPlane)
{
	return planeToCircle(pPlane, pCircle);
}

bool PhysicsScene::circleToCircle(PhysicsObject* pCircle1, PhysicsObject* pCircle2)
{
	// Dynamically casts the physics objects to circle objects
	Circle* circle1 = dynamic_cast<Circle*>(pCircle1);
	Circle* circle2 = dynamic_cast<Circle*>(pCircle2);

	if (!(circle1 == nullptr || circle2 == nullptr))
	{
		// If one of the objects isnt solid return
		if (!circle1->getSolid() || !circle2->getSolid())
			return false;

		// Compares the distance between the objects and the combined radius
		float distance = glm::distance(circle1->getPosition(), circle2->getPosition());
		if (distance < circle1->getRadius() + circle2->getRadius())
		{
			// Resitution
			distance = circle1->getRadius() + circle2->getRadius() - distance;
			glm::vec2 collisionNormal = glm::normalize(circle1->getPosition() - circle2->getPosition());
			circle1->setPosition(circle1->getPosition() + (collisionNormal * distance * 0.5f));
			circle2->setPosition(circle2->getPosition() - (collisionNormal * distance * 0.5f));

			circle1->resolveCollision(circle2, 0.5f * (circle1->getPosition() + circle2->getPosition()));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::circleToAABB(PhysicsObject* pCircle, PhysicsObject* pAABB)
{
	return false;
}

// Redundant, call counterpart
bool PhysicsScene::AABBToPlane(PhysicsObject* pAABB, PhysicsObject* pPlane)
{
	return planeToAABB(pPlane, pAABB);
}

// Redundant, call counterpart
bool PhysicsScene::AABBToCircle(PhysicsObject* pAABB, PhysicsObject* pCircle)
{
	return circleToAABB(pCircle, pAABB);
}

bool PhysicsScene::AABBToAABB(PhysicsObject* pAABB1, PhysicsObject* pAABB2)
{
	AABB* AABB1 = dynamic_cast<AABB*>(pAABB1);
	AABB* AABB2 = dynamic_cast<AABB*>(pAABB2);

	if (!(AABB1 == nullptr || AABB2 == nullptr))
	{
		// If one of the objects isn't solid return
		if (!AABB1->getSolid() || !AABB2->getSolid())
			return false;

		// Get the top right and bottom left corners of the AABBs
		// AABB1 top right
		glm::vec2 max1(AABB1->getPosition().x + (AABB1->getWidth() * 0.5f),
			AABB1->getPosition().y + (AABB1->getHeight() * 0.5f));
		// AABB1 bottom left
		glm::vec2 min1(AABB1->getPosition().x - (AABB1->getWidth() * 0.5f),
			AABB1->getPosition().y - (AABB1->getHeight() * 0.5f));
		// AABB2 top right
		glm::vec2 max2(AABB2->getPosition().x + (AABB2->getWidth() * 0.5f),
			AABB2->getPosition().y + (AABB2->getHeight() * 0.5f));
		// AABB2 bottom left
		glm::vec2 min2(AABB2->getPosition().x - (AABB2->getWidth() * 0.5f),
			AABB2->getPosition().y - (AABB2->getHeight() * 0.5f));

		if (min1.x <= max2.x && max1.x >= min2.x && min1.y <= max2.y && max1.y >= min2.y)
		{
			float x1 = max2.x - min1.x;
			float x2 = max1.x - min2.x;
			float y1 = max2.y - min1.y;
			float y2 = max1.y - min2.y;

			// Find the lowest value
			float smallest = x1;
			if (smallest > x2)
				smallest = x2;
			if (smallest > y1)
				smallest = y1;
			if (smallest > y2)
				smallest = y2;

			if (smallest == x1)			// Move right
			{
				AABB1->setPosition(AABB1->getPosition() + glm::vec2(1, 0) * smallest * 0.5f);
				AABB2->setPosition(AABB2->getPosition() - glm::vec2(1, 0) * smallest * 0.5f);
				AABB1->resolveCollision(AABB2, glm::vec2(1, 0));
			}
			else if (smallest == x2)		// Move left
			{
				AABB1->setPosition(AABB1->getPosition() + glm::vec2(-1, 0) * smallest * 0.5f);
				AABB2->setPosition(AABB2->getPosition() - glm::vec2(-1, 0) * smallest * 0.5f);
				AABB1->resolveCollision(AABB2, glm::vec2(-1, 0));
			}
			else if (smallest == y1)		// Move up
			{
				AABB1->setPosition(AABB1->getPosition() + glm::vec2(0, 1) * smallest * 0.5f);
				AABB2->setPosition(AABB2->getPosition() - glm::vec2(0, 1) * smallest * 0.5f);
				AABB1->resolveCollision(AABB2, glm::vec2(0, 1));
			}
			else if (smallest == y2)		// Move down
			{
				AABB1->setPosition(AABB1->getPosition() + glm::vec2(0, -1) * smallest * 0.5f);
				AABB2->setPosition(AABB2->getPosition() - glm::vec2(0, -1) * smallest * 0.5f);
				AABB1->resolveCollision(AABB2, glm::vec2(0, -1));
			}
			return true;
		}

		//glm::vec2 overlap(0, 0);

		//if (min1.x <= max2.x && max1.x >= min2.x && min1.y <= max2.y && max1.y >= min2.y)
		//{
		//	glm::vec2 temp(glm::vec2(max1.x, min1.y) - glm::vec2(min2.x, max2.y));

		//	if ((glm::dot(overlap, overlap) > glm::dot(temp, temp)) || (overlap == glm::vec2(0, 0)))
		//		overlap = temp;
		//}

		//if (overlap != glm::vec2(0, 0))
		//{
		//	std::cout << overlap.x << ", " << overlap.y << std::endl;

		//	glm::vec2 adjustment = overlap;

		//	if (glm::abs(overlap.x) < glm::abs(overlap.y))
		//		adjustment *= glm::vec2(-1, 0);
		//	else
		//		adjustment *= glm::vec2(0, -1);

		//	std::cout << adjustment.x << ", " << adjustment.y << std::endl;

		//	AABB1->setPosition(AABB1->getPosition() + adjustment / 2.0f);
		//	AABB2->setPosition(AABB2->getPosition() - adjustment / 2.0f);

		//	//// Testing the relative direction of one object relative to the other to work out which way they should bounce
		//	if (glm::abs(adjustment.x) > glm::abs(adjustment.y)) // Needs to be adjusted in the x axis
		//	{
		//		if (overlap.x > 0)	// Needs to be moved up
		//			AABB1->resolveCollision(AABB2, glm::vec2(0, 1));
		//		else	// Needs to be moved down
		//			AABB1->resolveCollision(AABB2, glm::vec2(0, -1));
		//	}
		//	else	 // Needs to be adjusted in the y axis
		//	{
		//		if (overlap.y > 0)	// Needs to be moved right
		//			AABB1->resolveCollision(AABB2, glm::vec2(1, 0));
		//		else	// Needs to be moved left
		//			AABB1->resolveCollision(AABB2, glm::vec2(-1, 0));
		//	}

		//	//AABB1->resolveCollision(AABB2, 0.5f * (AABB1->getPosition() + AABB2->getPosition()));

		//	return true;
		//}
	}
	return false;
}