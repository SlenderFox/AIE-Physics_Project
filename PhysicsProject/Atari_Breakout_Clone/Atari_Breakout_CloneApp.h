#pragma once
#include "Application.h"
#include "Renderer2D.h"

class Atari_Breakout_CloneApp : public aie::Application
{
public:
	Atari_Breakout_CloneApp();
	virtual ~Atari_Breakout_CloneApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};