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
		// If the objects isnt solid return
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
	Plane* plane = dynamic_cast<Plane*>(pPlane);
	AABB* aabb = dynamic_cast<AABB*>(pAABB);

	if (!(plane == nullptr || aabb == nullptr))
	{
		// If the objects isnt solid return
		if (!aabb->getSolid())
			return false;

		glm::vec2 collisionNormal = plane->getNormal();

		// v1  v2
		//    []
		// v4  v3
		// Top left
		glm::vec2 v1(aabb->getWidth() * -0.5f, aabb->getHeight() * 0.5f);
		// Top right
		glm::vec2 v2(aabb->getWidth() * 0.5f, aabb->getHeight() * 0.5f);
		// Bottom right
		glm::vec2 v3(aabb->getWidth() * 0.5f, aabb->getHeight() * -0.5f);
		// Bottom left
		glm::vec2 v4(aabb->getWidth() * -0.5f, aabb->getHeight() * -0.5f);

		// Center distance
		float centerToPlane = glm::dot(aabb->getPosition(), plane->getNormal()) - plane->getDistance();
		// Top left
		float v1DistanceToPlane = glm::dot(aabb->getPosition() + v1, plane->getNormal()) - plane->getDistance();
		// Top right
		float v2DistanceToPlane = glm::dot(aabb->getPosition() + v2, plane->getNormal()) - plane->getDistance();
		// Bottom right
		float v3DistanceToPlane = glm::dot(aabb->getPosition() + v3, plane->getNormal()) - plane->getDistance();
		// Bottom left
		float v4DistanceToPlane = glm::dot(aabb->getPosition() + v4, plane->getNormal()) - plane->getDistance();

		float tempv1 = v1DistanceToPlane;
		if (tempv1 < 0)
			tempv1 *= -1;
		float tempv2 = v2DistanceToPlane;
		if (tempv2 < 0)
			tempv2 *= -1;
		float tempv3 = v3DistanceToPlane;
		if (tempv3 < 0)
			tempv3 *= -1;
		float tempv4 = v4DistanceToPlane;
		if (tempv4 < 0)
			tempv4 *= -1;

		// Find the lowest value
		float smallest = tempv1;
		if (smallest > tempv2)
			smallest = tempv2;
		if (smallest > tempv3)
			smallest = tempv3;
		if (smallest > tempv4)
			smallest = tempv4;

		float intersection;
		glm::vec2 contact;

		if (smallest == tempv1)			// Top left is closest
		{
			// If we are behind plane then we flip the normal
			if (v1DistanceToPlane < 0)
			{
				collisionNormal *= -1;
				v1DistanceToPlane *= -1;
			}
			intersection = glm::abs(glm::dot(v1, collisionNormal)) - glm::abs(centerToPlane);
			if (intersection > 0)
			{
				// Resitution
				aabb->setPosition(aabb->getPosition() + collisionNormal * -intersection);

				contact = aabb->getPosition() + (collisionNormal * -glm::length(aabb->getPosition() - v1));

				plane->resolveCollision(aabb, contact);
				return true;
			}
		}
		else if (smallest == tempv2)		// Top right is closest
		{
			// If we are behind plane then we flip the normal
			if (v2DistanceToPlane < 0)
			{
				collisionNormal *= -1;
				v2DistanceToPlane *= -1;
			}
			intersection = glm::abs(glm::dot(v2, collisionNormal)) - glm::abs(centerToPlane);
			if (intersection > 0)
			{
				// Resitution
				aabb->setPosition(aabb->getPosition() + collisionNormal * -intersection);

				contact = aabb->getPosition() + (collisionNormal * -glm::length(aabb->getPosition() - v2));

				plane->resolveCollision(aabb, contact);
				return true;
			}
		}
		else if (smallest == tempv3)		// Bottom left is closest
		{
			// If we are behind plane then we flip the normal
			if (v3DistanceToPlane < 0)
			{
				collisionNormal *= -1;
				v3DistanceToPlane *= -1;
			}
			intersection = glm::abs(glm::dot(v3, collisionNormal)) - glm::abs(centerToPlane);
			if (intersection > 0)
			{
				// Resitution
				aabb->setPosition(aabb->getPosition() + collisionNormal * -intersection);

				contact = aabb->getPosition() + (collisionNormal * -glm::length(aabb->getPosition() - v3));

				plane->resolveCollision(aabb, contact);
				return true;
			}
		}
		else if (smallest == tempv4)		// Bottom right is closest
		{
			// If we are behind plane then we flip the normal
			if (v4DistanceToPlane < 0)
			{
				collisionNormal *= -1;
				v4DistanceToPlane *= -1;
			}
			intersection = glm::abs(glm::dot(v4, collisionNormal)) - glm::abs(centerToPlane);
			if (intersection > 0)
			{
				// Resitution
				aabb->setPosition(aabb->getPosition() + collisionNormal * -intersection);

				contact = aabb->getPosition() + (collisionNormal * -glm::length(aabb->getPosition() - v4));

				plane->resolveCollision(aabb, contact);
				return true;
			}
		}
	}
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
	// Dynamically casts the physics objects to circle objects
	Circle* circle = dynamic_cast<Circle*>(pCircle);
	AABB* aabb = dynamic_cast<AABB*>(pAABB);

	if (!(circle == nullptr || aabb == nullptr))
	{
		// If one of the objects isnt solid return
		if (!circle->getSolid() || !aabb->getSolid())
			return false;

		// Test if circle is inside the aabb
		if (!(circle->getPosition().x < aabb->getPosition().x - (aabb->getWidth() * 0.5f)
			|| circle->getPosition().x > aabb->getPosition().x + (aabb->getWidth() * 0.5f)
			|| circle->getPosition().y < aabb->getPosition().y - (aabb->getHeight() * 0.5f)
			|| circle->getPosition().y > aabb->getPosition().y + (aabb->getHeight() * 0.5f)))
		{
			// Restitution
			glm::vec2 collisionNormal = glm::normalize(circle->getPosition() - aabb->getPosition());
			circle->setPosition(circle->getPosition() + collisionNormal);

			circle->resolveCollision(aabb, collisionNormal);
			return true;
		}

		// Test if the circle should collide with a corner
		if (!(circle->getPosition().x > aabb->getPosition().x - (aabb->getWidth() * 0.5f)
			|| circle->getPosition().y < aabb->getPosition().y + (aabb->getHeight() * 0.5f)))
		{
			// Gets necessary variables for a circle to point collision
			glm::vec2 v1(aabb->getPosition().x + aabb->getWidth() * -0.5f, aabb->getPosition().y + aabb->getHeight() * 0.5f);
			glm::vec2 collisionNormal = glm::normalize(circle->getPosition() - v1);
			float circleToCorner = glm::length((circle->getPosition() + collisionNormal * circle->getRadius()) - v1);

			float intersection = circle->getRadius() - circleToCorner;
			if (intersection > 0)
			{
				// Resitution
				circle->setPosition(circle->getPosition() + collisionNormal * intersection);

				glm::vec2 contact = circle->getPosition() + (collisionNormal * -circle->getRadius());

				circle->resolveCollision(aabb, collisionNormal);
				return true;
			}
		}
		// Top right corner
		if (!(circle->getPosition().x < aabb->getPosition().x + (aabb->getWidth() * 0.5f)
			|| circle->getPosition().y < aabb->getPosition().y + (aabb->getHeight() * 0.5f)))
		{
			// Gets necessary variables for a circle to point collision
			glm::vec2 v2(aabb->getPosition().x + aabb->getWidth() * 0.5f, aabb->getPosition().y + aabb->getHeight() * 0.5f);
			glm::vec2 collisionNormal = glm::normalize(circle->getPosition() - v2);
			float circleToCorner = glm::length((circle->getPosition() + collisionNormal * circle->getRadius()) - v2);

			float intersection = circle->getRadius() - circleToCorner;
			if (intersection > 0)
			{
				// Resitution
				circle->setPosition(circle->getPosition() + collisionNormal * intersection);

				glm::vec2 contact = circle->getPosition() + (collisionNormal * -circle->getRadius());

				circle->resolveCollision(aabb, collisionNormal);
				return true;
			}
			return true;
		}
		// Bottom left corner
		if (!(circle->getPosition().x > aabb->getPosition().x - (aabb->getWidth() * 0.5f)
			|| circle->getPosition().y > aabb->getPosition().y - (aabb->getHeight() * 0.5f)))
		{
			// Gets necessary variables for a circle to point collision
			glm::vec2 v3(aabb->getPosition().x + aabb->getWidth() * 0.5f, aabb->getPosition().y + aabb->getHeight() * -0.5f);
			glm::vec2 collisionNormal = glm::normalize(circle->getPosition() - v3);
			float circleToCorner = glm::length((circle->getPosition() + collisionNormal * circle->getRadius()) - v3);

			float intersection = circle->getRadius() - circleToCorner;
			if (intersection > 0)
			{
				// Resitution
				circle->setPosition(circle->getPosition() + collisionNormal * intersection);

				glm::vec2 contact = circle->getPosition() + (collisionNormal * -circle->getRadius());

				circle->resolveCollision(aabb, collisionNormal);
				return true;
			}
			return true;
		}
		// Bottom right corner
		if (!(circle->getPosition().x < aabb->getPosition().x + (aabb->getWidth() * 0.5f)
			|| circle->getPosition().y > aabb->getPosition().y - (aabb->getHeight() * 0.5f)))
		{
			// Gets necessary variables for a circle to point collision
			glm::vec2 v4(aabb->getPosition().x + aabb->getWidth() * -0.5f, aabb->getPosition().y + aabb->getHeight() * -0.5f);
			glm::vec2 collisionNormal = glm::normalize(circle->getPosition() - v4);
			float circleToCorner = glm::length((circle->getPosition() + collisionNormal * circle->getRadius()) - v4);

			float intersection = circle->getRadius() - circleToCorner;
			if (intersection > 0)
			{
				// Resitution
				circle->setPosition(circle->getPosition() + collisionNormal * intersection);

				glm::vec2 contact = circle->getPosition() + (collisionNormal * -circle->getRadius());

				circle->resolveCollision(aabb, collisionNormal);
				return true;
			}
			return true;
		}

		// Test if the circle should collide with an edge
		if ((!(circle->getPosition().x < aabb->getPosition().x - (aabb->getWidth() * 0.5f)
			|| circle->getPosition().x > aabb->getPosition().x + (aabb->getWidth() * 0.5f)
			|| circle->getPosition().y < aabb->getPosition().y + (aabb->getHeight() * 0.5f)))
			|| (!(circle->getPosition().x < aabb->getPosition().x - (aabb->getWidth() * 0.5f)
				|| circle->getPosition().x > aabb->getPosition().x + (aabb->getWidth() * 0.5f)
				|| circle->getPosition().y > aabb->getPosition().y - (aabb->getHeight() * 0.5f)))
			|| (!(circle->getPosition().x < aabb->getPosition().x + (aabb->getWidth() * 0.5f)
				|| circle->getPosition().y > aabb->getPosition().y + (aabb->getHeight() * 0.5f)
				|| circle->getPosition().y < aabb->getPosition().y - (aabb->getHeight() * 0.5f)))
			|| (!(circle->getPosition().x > aabb->getPosition().x - (aabb->getWidth() * 0.5f)
				|| circle->getPosition().y > aabb->getPosition().y + (aabb->getHeight() * 0.5f)
				|| circle->getPosition().y < aabb->getPosition().y - (aabb->getHeight() * 0.5f)))
			)
		{
			AABB* circleBB = new AABB(circle->getPosition(), glm::vec2(0, 0), 
				circle->getMass(), circle->getElasticity(),
				0,
				true, false,
				circle->getRadius() * 2, circle->getRadius() * 2, glm::vec4(1, 1, 1, 1));

			// Get the top right and bottom left corners of the AABBs
			// AABB1 top right
			glm::vec2 max1(aabb->getPosition().x + (aabb->getWidth() * 0.5f),
				aabb->getPosition().y + (aabb->getHeight() * 0.5f));
			// AABB1 bottom left
			glm::vec2 min1(aabb->getPosition().x - (aabb->getWidth() * 0.5f),
				aabb->getPosition().y - (aabb->getHeight() * 0.5f));
			// AABB2 top right
			glm::vec2 max2(circleBB->getPosition().x + (circleBB->getWidth() * 0.5f),
				circleBB->getPosition().y + (circleBB->getHeight() * 0.5f));
			// AABB2 bottom left
			glm::vec2 min2(circleBB->getPosition().x - (circleBB->getWidth() * 0.5f),
				circleBB->getPosition().y - (circleBB->getHeight() * 0.5f));

			delete circleBB;

			if (!(!(min1.x <= max2.x) || !(max1.x >= min2.x) || !(min1.y <= max2.y) || !(max1.y >= min2.y)))
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

				glm::vec2 collisionNormal;

				if (smallest == x1)			// Move right
				{
					collisionNormal = glm::vec2(1, 0);
					aabb->setPosition(aabb->getPosition() + collisionNormal * smallest * 0.5f);
					circle->setPosition(circle->getPosition() - collisionNormal * smallest * 0.5f);
					aabb->resolveCollision(circle, &collisionNormal);
				}
				else if (smallest == x2)		// Move left
				{
					collisionNormal = glm::vec2(-1, 0);
					aabb->setPosition(aabb->getPosition() + collisionNormal * smallest * 0.5f);
					circle->setPosition(circle->getPosition() - collisionNormal * smallest * 0.5f);
					aabb->resolveCollision(circle, &collisionNormal);
				}
				else if (smallest == y1)		// Move up
				{
					collisionNormal = glm::vec2(0, 1);
					aabb->setPosition(aabb->getPosition() + collisionNormal * smallest * 0.5f);
					circle->setPosition(circle->getPosition() - collisionNormal * smallest * 0.5f);
					aabb->resolveCollision(circle, &collisionNormal);
				}
				else if (smallest == y2)		// Move down
				{
					collisionNormal = glm::vec2(0, -1);
					aabb->setPosition(aabb->getPosition() + collisionNormal * smallest * 0.5f);
					circle->setPosition(circle->getPosition() - collisionNormal * smallest * 0.5f);
					aabb->resolveCollision(circle, &collisionNormal);
				}
				return true;
			}
		}
	}
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
	AABB* aabb1 = dynamic_cast<AABB*>(pAABB1);
	AABB* aabb2 = dynamic_cast<AABB*>(pAABB2);

	if (!(aabb1 == nullptr || aabb2 == nullptr))
	{
		// If one of the objects isn't solid return
		if (!aabb1->getSolid() || !aabb2->getSolid())
			return false;

		// Get the top right and bottom left corners of the AABBs
		// AABB1 top right
		glm::vec2 max1(aabb1->getPosition().x + (aabb1->getWidth() * 0.5f),
			aabb1->getPosition().y + (aabb1->getHeight() * 0.5f));
		// AABB1 bottom left
		glm::vec2 min1(aabb1->getPosition().x - (aabb1->getWidth() * 0.5f),
			aabb1->getPosition().y - (aabb1->getHeight() * 0.5f));
		// AABB2 top right
		glm::vec2 max2(aabb2->getPosition().x + (aabb2->getWidth() * 0.5f),
			aabb2->getPosition().y + (aabb2->getHeight() * 0.5f));
		// AABB2 bottom left
		glm::vec2 min2(aabb2->getPosition().x - (aabb2->getWidth() * 0.5f),
			aabb2->getPosition().y - (aabb2->getHeight() * 0.5f));

		if (!(!(min1.x <= max2.x) || !(max1.x >= min2.x) || !(min1.y <= max2.y) || !(max1.y >= min2.y)))
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

			glm::vec2 collisionNormal;

			if (smallest == x1)			// Move right
			{
				collisionNormal = glm::vec2(1, 0);
				aabb1->setPosition(aabb1->getPosition() + collisionNormal * smallest * 0.5f);
				aabb2->setPosition(aabb2->getPosition() - collisionNormal * smallest * 0.5f);
				aabb1->resolveCollision(aabb2, &collisionNormal);
			}
			else if (smallest == x2)		// Move left
			{
				collisionNormal = glm::vec2(-1, 0);
				aabb1->setPosition(aabb1->getPosition() + collisionNormal * smallest * 0.5f);
				aabb2->setPosition(aabb2->getPosition() - collisionNormal * smallest * 0.5f);
				aabb1->resolveCollision(aabb2, &collisionNormal);
			}
			else if (smallest == y1)		// Move up
			{
				collisionNormal = glm::vec2(0, 1);
				aabb1->setPosition(aabb1->getPosition() + collisionNormal * smallest * 0.5f);
				aabb2->setPosition(aabb2->getPosition() - collisionNormal * smallest * 0.5f);
				aabb1->resolveCollision(aabb2, &collisionNormal);
			}
			else if (smallest == y2)		// Move down
			{
				collisionNormal = glm::vec2(0, -1);
				aabb1->setPosition(aabb1->getPosition() + collisionNormal * smallest * 0.5f);
				aabb2->setPosition(aabb2->getPosition() - collisionNormal * smallest * 0.5f);
				aabb1->resolveCollision(aabb2, &collisionNormal);
			}
			return true;
		}
	}
	return false;
}