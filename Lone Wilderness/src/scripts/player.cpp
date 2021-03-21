/*! \file player.cpp
*
* \brief A player dummy class
*
* \author: Daniel Bullin
*
*/
#include "scripts/player.h"
#include "loaders/sceneLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/eventManager.h"
#include "independent/systems/systems/threadManager.h"

//! Player()
Player::Player()
{
	m_controller = nullptr;
}

//! ~Player()
Player::~Player()
{
}

void Player::testFunction(int& y)
{
	ENGINE_INFO("Test: {0}", y);
}

void Player::onAttach()
{
	m_controller = getParent()->getComponent<CharacterController>();

	auto t = std::bind(&Player::testFunction, this, std::placeholders::_1);
	int y = 2;
	ThreadManager::startThread("Thread1", true, t, y);
}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onPreUpdate(const float timestep, const float totalTime)
{
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onPostUpdate(const float timestep, const float totalTime)
{
}

//! onKeyPress()
/*!
\param e a KeyPressedEvent& - A key press event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::W)
	{
		if (m_controller)
			m_controller->move(FORWARD, timestep);
	}
	if (e.getKeyCode() == Keys::A)
	{
		if (m_controller)
			m_controller->move(LEFT, timestep);
	}
	if (e.getKeyCode() == Keys::S)
	{
		if (m_controller)
			m_controller->move(BACKWARD, timestep);
	}
	if (e.getKeyCode() == Keys::D)
	{
		if (m_controller)
			m_controller->move(RIGHT, timestep);
	}
	if (e.getKeyCode() == Keys::SPACE)
	{
		if(m_controller)
			m_controller->move(UP, timestep);
	}
	if (e.getKeyCode() == Keys::LEFT_CONTROL)
	{
		if (m_controller)
			m_controller->move(DOWN, timestep);
	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::B)
	{
		getParent()->getParentScene()->getEntity("Cyborg1")->getComponent<MeshRender3D>()->setModel(ResourceManager::getResource<Model3D>("cube2"));
	}
}

//! onMouseScrolled()
/*!
\param e a MouseScrolledEvent& - A mouse scrolled event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime)
{
	if (m_controller)
		m_controller->zoom(e.getYOffset());


}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
	if (m_controller)
		m_controller->rotate(EventManager::getEventData().mouseOffset.x, EventManager::getEventData().mouseOffset.y);
}