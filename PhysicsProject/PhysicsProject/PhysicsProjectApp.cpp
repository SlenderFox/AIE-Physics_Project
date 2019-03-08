#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include <vector>
#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"

PhysicsProjectApp::PhysicsProjectApp()
{
}

PhysicsProjectApp::~PhysicsProjectApp()
{
}

bool PhysicsProjectApp::startup()
{
	srand((unsigned int)time(NULL));
	m_rocketTimer = 0;

	// Startup options:
	m_rocketEnabled = false;
	m_newtonsCradleEnabled = false;
	m_massExampleEnabled = true;
	m_ballChaos = false;
	//----------

	// Increase the 2d line count to maximize the number of objects we can draw 
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	// Lower timestep results in higher precision
	m_physicsScene->setTimeStep(0.01f);
	// Set gravity to irl gravity
	m_physicsScene->setGravity(glm::vec2(0, -9.8));

	// Creates a bounding box around the window
	float PI = 3.14159265358979323846264338327950388f;		// Complete overkill lmao
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 0.0f), cosf(PI * 0.0f)), -55.5f, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 0.5f), cosf(PI * 0.5f)), -99.0f, glm::vec4(0, 1, 0, 1)));
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 1.0f), cosf(PI * 1.0f)), -55.5f, glm::vec4(0, 0, 1, 1)));
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 1.5f), cosf(PI * 1.5f)), -99.0f, glm::vec4(1, 1, 0, 1)));
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 1.25f), cosf(PI * 1.25f)), 0.0f, glm::vec4(1, 1, 1, 1)));

	m_physicsScene->addActor(new AABB(glm::vec2(-30, 0), glm::vec2(0, -10),
		10.0f, 1.0f,
		0,
		true, false,
		12.0f, 8.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1)));

	m_physicsScene->addActor(new AABB(glm::vec2(-25, -20), glm::vec2(0, 0),
		10.0f, 1.0f,
		0,
		true, false,
		12.0f, 8.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1)));

	m_physicsScene->addActor(new AABB(glm::vec2(-50, -20), glm::vec2(10, 0),
		10.0f, 1.0f,
		0,
		true, false,
		12.0f, 8.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1)));

	m_physicsScene->addActor(new AABB(glm::vec2(10, -25), glm::vec2(-10, 0),
		10.0f, 1.0f,
		0,
		true, false,
		12.0f, 8.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1)));

	m_physicsScene->addActor(new AABB(glm::vec2(-35, -40), glm::vec2(0, 0),
		10.0f, 1.0f,
		0,
		true, false,
		12.0f, 8.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1)));

	if (m_rocketEnabled)
	{
		// Creates a rocket
		m_physicsScene->addActor(new Circle(glm::vec2(40, -30), glm::vec2(0, 0),
			0.0f, 0.0f,
			50.0f, 0.25f,
			0.0f, 0.0f,
			true, true,
			5.0f, glm::vec4(0.753f, 0.753f, 0.753f, 1)));
	}

	if (m_newtonsCradleEnabled)
	{
		// Creates a crappy newtons cradle that only works once
		m_physicsScene->addActor(new Circle(glm::vec2(-65, -30), glm::vec2(0, 0),
			0.0f, 0.0f,
			2.0f, 1.0f,
			0.0f, 0.0f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-60, -30), glm::vec2(0, 0),
			0.0f, 0.0f,
			2.0f, 1.0f,
			0.0f, 0.0f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-55, -30), glm::vec2(0, 0),
			0.0f, 0.0f,
			2.0f, 1.0f,
			0.0f, 0.0f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-50, -30), glm::vec2(0, 0),
			0.0f, 0.0f,
			2.0f, 1.0f,
			0.0f, 0.0f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
	}

	if (m_massExampleEnabled)
	{
		//// Creates a head on collision between 2 circles of the same mass
		//m_physicsScene->addActor(new Circle(glm::vec2(-80, 40), glm::vec2(20, 0),
		//	0.0f, 0.0f,
		//	2.0f, 0.5f,
		//	0.0f, 0.0f,
		//	true, false,
		//	2.0f, glm::vec4(1, 0, 0, 1)));
		//m_physicsScene->addActor(new Circle(glm::vec2(-40, 40), glm::vec2(-20, 0),
		//	0.0f, 0.0f,
		//	2.0f, 0.5f,
		//	0.0f, 0.0f,
		//	true, false,
		//	2.0f, glm::vec4(1, 0, 0, 1)));

		//// Creates a head on collision between 2 circles of different masses
		//m_physicsScene->addActor(new Circle(glm::vec2(-80, 30), glm::vec2(20, 5),
		//	0.0f, 0.0f,
		//	1.8f, 0.5f,
		//	0.0f, 0.0f,
		//	true, false,
		//	1.8f, glm::vec4(1, 0, 0, 1)));
		//m_physicsScene->addActor(new Circle(glm::vec2(-40, 30), glm::vec2(-20, 5),
		//	0.0f, 0.0f,
		//	3.0f, 0.5f,
		//	0.0f, 0.0f,
		//	true, false,
		//	3.0f, glm::vec4(1, 0, 0, 1)));

		// Creates a collision of two similar objectes at an offset
		m_physicsScene->addActor(new Circle(glm::vec2(-90, 23), glm::vec2(20, 0),
			0.0f, 0.0f,
			5.0f, 1.0f,
			0.0f, 0.0f,
			true, false,
			5.0f, glm::vec4(1, 0, 0, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-30, 20), glm::vec2(-20, 0),
			0.0f, 0.0f,
			5.0f, 1.0f,
			0.0f, 0.0f,
			true, false,
			5.0f, glm::vec4(1, 0, 0, 1)));
	}

	if (m_ballChaos)
	{
		int smallBalls = 25;
		int mediumBalls = 10;
		int largeBalls = 5;
		bool gravityEnabled = false;
		float linearDrag = 0.0001f;
		float angularDrag = 0.0f;
		float elasticity = 1.0f;

		// Creates small balls
		for (int i = 0; i < smallBalls; i++)
		{
			m_physicsScene->addActor(new Circle(glm::vec2(rand() % 180 - 90, rand() % 90 - 45),
				glm::vec2(rand() % 200 - 100, rand() % 200 - 100),
				0.0f, 0.0f,
				1.0f, elasticity,
				linearDrag, angularDrag,
				true, gravityEnabled,
				1.5f, glm::vec4(0, 1, 0, 1)));
		}

		// Creates medium balls
		for (int i = 0; i < mediumBalls; i++)
		{
			m_physicsScene->addActor(new Circle(glm::vec2(rand() % 180 - 90, rand() % 90 - 45),
				glm::vec2(rand() % 200 - 100, rand() % 200 - 100),
				0.0f, 0.0f,
				3.0f, elasticity * 0.80f,
				linearDrag, angularDrag,
				true, gravityEnabled,
				3.5f, glm::vec4(0, 0, 1, 1)));
		}

		// Creates large balls
		for (int i = 0; i < largeBalls; i++)
		{
			m_physicsScene->addActor(new Circle(glm::vec2(rand() % 180 - 90, rand() % 90 - 45),
				glm::vec2(rand() % 200 - 100, rand() % 200 - 100),
				0.0f, 0.0f,
				5.0f, elasticity * 0.60f,
				linearDrag, angularDrag,
				true, gravityEnabled,
				5.5f, glm::vec4(1, 0, 0, 1)));
		}
	}

	return true;
}

void PhysicsProjectApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void PhysicsProjectApp::update(float pDeltaTime)
{
	// Input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	// Rocket
	if (m_rocketEnabled)
	{
		if (m_rocketTimer < 0.1f)
			m_rocketTimer += pDeltaTime;

		if (input->isKeyDown(aie::INPUT_KEY_SPACE) && m_rocketTimer >= 0.1f)
		{
			// As long as the rocket has enough mass force the rocket forward using exhaust
			Rigidbody* rocket = dynamic_cast<Rigidbody*>(m_physicsScene->getActor(0));
			if (rocket->getMass() > 1.0f)
			{
				rocket->setMass(rocket->getMass() - 1.0f);

				// Creates an exhaust particle at the base of the rocket
				Circle* rocketExhaust = new Circle(glm::vec2(rocket->getPosition().x, rocket->getPosition().y - 5.5f), glm::vec2(0, 0),
					1.0f, 1.0f,
					0.0f, 0.0f,
					0.3f, 0.3f,
					false, false,
					0.25f, glm::vec4(0.961f, 0.961f, 0.961f, 1));

				// Applies a force to the rocket and its exhaust implying a chemical force
				glm::vec2 exhaustForce(0, 10.0f);
				rocket->applyForce(exhaustForce);
				rocketExhaust->applyForce(-exhaustForce);

				m_physicsScene->addActor(rocketExhaust);
				m_rocketTimer -= 0.1f;
			}
		}
	}

	// Updates all actors
	m_physicsScene->update(pDeltaTime);
	m_physicsScene->updateGizmos();

	// Exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsProjectApp::draw()
{
	// Wipe the screen to the background colour
	clearScreen();

	// Begin drawing sprites
	m_2dRenderer->begin();

	// Draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// Output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// Done drawing sprites
	m_2dRenderer->end();
}