#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include <glm\vec2.hpp>

class PhysicsScene;

class ProjectilePhysicsApp : public aie::Application
{
public:
	ProjectilePhysicsApp();
	virtual ~ProjectilePhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);

protected:
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene*		m_physicsScene;
};