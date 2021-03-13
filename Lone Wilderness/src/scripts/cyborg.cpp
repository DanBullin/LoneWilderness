/*! \file cyborg.cpp
*
* \brief A Cyborg dummy class
*
* \author: Daniel Bullin
*
*/
#include "scripts/cyborg.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/entities/entity.h"

//! Cyborg()
Cyborg::Cyborg()
{
	m_controller = nullptr;
}

//! ~Cyborg()
Cyborg::~Cyborg()
{
}

//! onPreUpdate()
/*
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onPreUpdate(const float timestep, const float totalTime)
{
	if (getName() == "CubeDemo")
	{
		auto transform = getParent()->getComponent<Transform>();

		if (originalScale == glm::vec3(0.f, 0.f, 0.f))
			originalScale = transform->getScale();

		transform->setScale(originalScale + glm::vec3(((sin(totalTime) + 1.f) / 2.f)));
	}
}

//! onKeyPress()
/*!
\param e a KeyPressedEvent& - A key press event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
	if (!m_controller) m_controller = getParent()->getComponent<CharacterController>();
	else
	{
		if (e.getKeyCode() == Keys::UP)
		{
			m_controller->move(FORWARD, timestep);
		}

		if (e.getKeyCode() == Keys::DOWN)
		{
			m_controller->move(BACKWARD, timestep);
		}

		if (e.getKeyCode() == Keys::LEFT)
		{
			m_controller->move(LEFT, timestep);
		}

		if (e.getKeyCode() == Keys::RIGHT)
		{
			m_controller->move(RIGHT, timestep);
		}

		if (e.getKeyCode() == Keys::SPACE)
		{
			m_controller->move(UP, timestep);
		}

		if (e.getKeyCode() == Keys::X)
		{
			m_controller->move(RIGHT, timestep);
		}
	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::P)
	{
		getParent()->setDisplay(!getParent()->getDisplay());
	}
}