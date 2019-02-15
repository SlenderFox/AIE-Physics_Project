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

PhysicsProjectApp::PhysicsProjectApp()
{
}

PhysicsProjectApp::~PhysicsProjectApp()
{
}

bool PhysicsProjectApp::startup()
{
	// Startup options:
	m_rocketEnabled = false;
	m_newtonsCradleEnabled = false;
	m_massExampleEnabled = false;
	m_ballChaos = true;
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
	float PI = 3.14159265f;
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 0.0f), cosf(PI * 0.0f)), 55.5f, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 0.5f), cosf(PI * 0.5f)), 99.0f, glm::vec4(0, 1, 0, 1)));
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 1.0f), cosf(PI * 1.0f)), 55.5f, glm::vec4(0, 0, 1, 1)));
	m_physicsScene->addActor(new Plane(glm::vec2(sinf(PI * 1.5f), cosf(PI * 1.5f)), 99.0f, glm::vec4(1, 1, 0, 1)));

	if (m_rocketEnabled)
	{
		// Creates a rocket
		m_physicsScene->addActor(new Circle(glm::vec2(40, -30), glm::vec2(0, 0),
			0.0f, 0.0f, 50.0f,
			0.3f, 0.3f,
			true, true,
			5.0f, glm::vec4(0.753f, 0.753f, 0.753f, 1)));
	}

	if (m_newtonsCradleEnabled)
	{
		// Creates a crappy newtons cradle that only works once
		m_physicsScene->addActor(new Circle(glm::vec2(-65, -30), glm::vec2(0, 0),
			0.0f, 0.0f, 2.0f,
			0.3f, 0.3f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-60, -30), glm::vec2(0, 0),
			0.0f, 0.0f, 2.0f,
			0.3f, 0.3f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-55, -30), glm::vec2(0, 0),
			0.0f, 0.0f, 2.0f,
			0.3f, 0.3f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-50, -30), glm::vec2(0, 0),
			0.0f, 0.0f, 2.0f,
			0.3f, 0.3f,
			true, false,
			2.0f, glm::vec4(0.31f, 0.141f, 0.071f, 1)));
	}

	if (m_massExampleEnabled)
	{
		// Creates a head on collision between 2 circles of the same mass
		m_physicsScene->addActor(new Circle(glm::vec2(-80, 40), glm::vec2(20, 0),
			0.0f, 0.0f, 2.0f,
			0.3f, 0.3f,
			true, false,
			2.0f, glm::vec4(1, 0, 0, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-40, 40), glm::vec2(-20, 0),
			0.0f, 0.0f, 2.0f,
			0.3f, 0.3f,
			true, false,
			2.0f, glm::vec4(1, 0, 0, 1)));

		// Creates a head on collision between 2 circles of different masses
		m_physicsScene->addActor(new Circle(glm::vec2(-80, 30), glm::vec2(20, 5),
			0.0f, 0.0f, 1.8f,
			0.3f, 0.3f,
			true, false,
			1.8f, glm::vec4(1, 0, 0, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-40, 30), glm::vec2(-20, 5),
			0.0f, 0.0f, 3.0f,
			0.3f, 0.3f,
			true, false,
			3.0f, glm::vec4(1, 0, 0, 1)));
	}

	if (m_ballChaos)
	{
		bool gravityEnabled = false;

		// Creates lots of balls with random attributes
		m_physicsScene->addActor(new Circle(glm::vec2(-74, -4), glm::vec2(43, 73),
			0.0f, 0.0f, 11.6f,
			0.3f, 0.3f,
			true, gravityEnabled,
			5.8f, glm::vec4(0.54f, 0.18f, 0.14f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(28, 33), glm::vec2(0.4f, -13),
			0.0f, 0.0f, 6.2f,
			0.3f, 0.3f,
			true, gravityEnabled,
			3.1f, glm::vec4(0.26f, 0.16f, 0.96f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-62, 23), glm::vec2(25, -56),
			0.0f, 0.0f, 3.6f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.8f, glm::vec4(0.27f, 0.26f, 0.60f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(86, -13), glm::vec2(-10, 12),
			0.0f, 0.0f, 6.4f,
			0.3f, 0.3f,
			true, gravityEnabled,
			3.2f, glm::vec4(0.17f, 0.27f, 0.76f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-46, 37), glm::vec2(13, 1.3f),
			0.0f, 0.0f, 15.4f,
			0.3f, 0.3f,
			true, gravityEnabled,
			7.7f, glm::vec4(0.02f, 0.92f, 0.06f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-11, 40), glm::vec2(-0.1f, -10),
			0.0f, 0.0f, 2.6f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.3f, glm::vec4(0.92f, 0.91f, 0.27f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-16, 14), glm::vec2(30, 25),
			0.0f, 0.0f, 18.0f,
			0.3f, 0.3f,
			true, gravityEnabled,
			9.0f, glm::vec4(0.75f, 0.31f, 0.48f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(79, -34), glm::vec2(-34, 93),
			0.0f, 0.0f, 2.0f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.0f, glm::vec4(0.76f, 0.17f, 0.35f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-97, -15), glm::vec2(-34, -17),
			0.0f, 0.0f, 3.4f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.7f, glm::vec4(0.25f, 0.91f, 0.94f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(76, 34), glm::vec2(49, 17),
			0.0f, 0.0f, 10.8f,
			0.3f, 0.3f,
			true, gravityEnabled,
			5.4f, glm::vec4(0.69f, 0.15f, 0.64f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-43, 25), glm::vec2(78, -25),
			0.0f, 0.0f, 3.8f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.6f, glm::vec4(0.07f, 0.54f, 0.27f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-57, -15), glm::vec2(-43, 27),
			0.0f, 0.0f, 2.6f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.3f, glm::vec4(0.17f, 0.26f, 0.69f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-10, -24), glm::vec2(-17, 35),
			0.0f, 0.0f, 15.6f,
			0.3f, 0.3f,
			true, gravityEnabled,
			7.8f, glm::vec4(0.54f, 0.34f, 0.51f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(76, 14), glm::vec2(-45, 76),
			0.0f, 0.0f, 8.6f,
			0.3f, 0.3f,
			true, gravityEnabled,
			4.3f, glm::vec4(0.98f, 0.18f, 0.05f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(45, -28), glm::vec2(-27, 14),
			0.0f, 0.0f, 10.8f,
			0.3f, 0.3f,
			true, gravityEnabled,
			5.4f, glm::vec4(0.04f, 0.76f, 0.24f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-79, 16), glm::vec2(-17, 24),
			0.0f, 0.0f, 2.8f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.4f, glm::vec4(0.02f, 0.37f, 0.54f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(14, -18), glm::vec2(-15, 27),
			0.0f, 0.0f, 4.8f,
			0.3f, 0.3f,
			true, gravityEnabled,
			2.4f, glm::vec4(0.17f, 0.94f, 0.14f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-30, 27), glm::vec2(-25, 76),
			0.0f, 0.0f, 3.4f,
			0.3f, 0.3f,
			true, gravityEnabled,
			1.7f, glm::vec4(0.27f, 0.53f, 0.64f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(5, -15), glm::vec2(-65, 68),
			0.0f, 0.0f, 6.0f,
			0.3f, 0.3f,
			true, gravityEnabled,
			3.0f, glm::vec4(0.71f, 0.46f, 0.41f, 1)));
		m_physicsScene->addActor(new Circle(glm::vec2(-56, 14), glm::vec2(-54, 37),
			0.0f, 0.0f, 9.2f,
			0.3f, 0.3f,
			true, gravityEnabled,
			4.6f, glm::vec4(0.92f, 0.81f, 0.25f, 1)));
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
					0.0f, 0.0f, 1.0f,
					0.3f, 0.3f,
					false, false,
					0.25f, glm::vec4(0.961f, 0.961f, 0.961f, 1));

				// Applies a force to the rocket and its exhaust implying a chemical force
				rocket->applyForceToActor(rocketExhaust, glm::vec2(0, 10.0f));

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