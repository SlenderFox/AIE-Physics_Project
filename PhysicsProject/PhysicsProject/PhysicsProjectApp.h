#pragma once
#include "Application.h"
#include "Renderer2D.h"

class PhysicsScene;

class PhysicsProjectApp : public aie::Application
{
public:
	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float pDeltaTime);
	virtual void draw();

protected:
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene*		m_physicsScene;

	// Startup options
	bool					m_rocketEnabled;				// Determines whether the rocket is created
	bool					m_newtonsCradleEnabled;	// Determines whether the newtons cradle is created
	bool					m_massExampleEnabled;	// Determines whether the collision demonstration is created
	bool					m_ballChaos;					// Determines whether the ball chaos demonstration is created

	float					m_rocketTimer;				// The timer controlling how often the rocket expels exhaust
};