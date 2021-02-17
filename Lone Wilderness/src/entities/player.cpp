/*! \file player.cpp
*
* \brief A player dummy class
*
* \author: Daniel Bullin
*
*/
#include "entities/player.h"
#include "independent/systems/systemManager.h"

//! Player()
Player::Player()
{
}

//! ~Player()
Player::~Player()
{
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
		if (containsComponent<Camera>())
			getParentScene()->getMainCamera()->move(FORWARD, timestep);
	}
	if (e.getKeyCode() == Keys::A)
	{
		if (containsComponent<Camera>())
			getParentScene()->getMainCamera()->move(LEFT, timestep);
	}
	if (e.getKeyCode() == Keys::S)
	{
		if (containsComponent<Camera>())
			getParentScene()->getMainCamera()->move(BACKWARD, timestep);
	}
	if (e.getKeyCode() == Keys::D)
	{
		if (containsComponent<Camera>())
			getParentScene()->getMainCamera()->move(RIGHT, timestep);
	}
	if (e.getKeyCode() == Keys::SPACE)
	{
		if(containsComponent<Camera>())
			getParentScene()->getMainCamera()->move(UP, timestep);
	}
	if (e.getKeyCode() == Keys::LEFT_CONTROL)
	{
		if (containsComponent<Camera>())
			getParentScene()->getMainCamera()->move(DOWN, timestep);
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
	if (containsComponent<Camera>())
		getParentScene()->getMainCamera()->zoom(e.getYOffset());
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
	if (containsComponent<Camera>())
		getParentScene()->getMainCamera()->rotate(EventManager::getEventData().mouseOffset.x, EventManager::getEventData().mouseOffset.y);
}