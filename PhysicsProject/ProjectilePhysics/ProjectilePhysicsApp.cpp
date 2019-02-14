#include "ProjectilePhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <glm\vec4.hpp>
#include "PhysicsScene.h"
#include "Circle.h"

ProjectilePhysicsApp::ProjectilePhysicsApp()
{
}

ProjectilePhysicsApp::~ProjectilePhysicsApp()
{
}

bool ProjectilePhysicsApp::startup()
{
	// Increase the 2d line count to maximize the number of objects we can draw 
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -9.8f));
	m_physicsScene->setTimeStep(0.1f);

	glm::vec2 startPos(-90, -50);
	float inclination = 0.5f;
	float speed = 40;
	float radius = 1.0f;

	m_physicsScene->addActor(new Circle(startPos, glm::vec2(sinf(inclination) * speed, cosf(inclination) * speed), 1.0f, radius, glm::vec4(1, 0, 0, 1), false));

	setupContinuousDemo(startPos, inclination, speed, -9.8f);

	return true;
}

void ProjectilePhysicsApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void ProjectilePhysicsApp::update(float deltaTime)
{
	// Input example
	aie::Input* input = aie::Input::getInstance();

	// Updates all actors
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// Exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void ProjectilePhysicsApp::draw()
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

void ProjectilePhysicsApp::setupContinuousDemo(glm::vec2 pPosition, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.2f;
	float radius = 1.0f;
	int segments = 32;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	glm::vec2 velocity(sinf(inclination) * speed, cosf(inclination)* speed);

	while (t < 10)
	{
		// Calculate the x, y position of the projectile at time t
		velocity.y += gravity * tStep;
		pPosition += velocity * tStep;

		aie::Gizmos::add2DCircle(glm::vec2(pPosition.x, pPosition.y), radius, segments, colour);
		t += tStep;
	}
}