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

void Player::onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime)
{
}

void Player::onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime)
{
}

void Player::onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime)
{
}

void Player::onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime)
{
}

void Player::onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::W)
	{
		Transform3D* trans = getComponent<Transform3D>();

		trans->setPosition({ trans->getPosition() + ((getComponent<Camera>()->getCameraData().Front * timestep * 10.f))});
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
}

void Player::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
	if (containsComponent<Camera>() && e.getKeyCode() == Keys::X)
	{
		SceneManager::setActiveScene("default", false);
	}

	if (containsComponent<Camera>() && e.getKeyCode() == Keys::C)
	{
		getParentScene()->getEntity("Quad")->getComponent<Text>()->printComponentDetails();
	}

	if (e.getKeyCode() == Keys::I)
	{
		SceneManager::setActiveScene("default", false);
		SceneManager::destroyScene("scene1");
	}

}

void Player::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
}

void Player::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	
}

void Player::onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime)
{
	if (containsComponent<Camera>())
		getParentScene()->getMainCamera()->zoom(e.getYOffset());
}

void Player::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
	if (containsComponent<Camera>())
		getParentScene()->getMainCamera()->rotate(EventManager::getEventData().mouseOffset.x, EventManager::getEventData().mouseOffset.y);

}

//! onRender()
void Player::onRender(const Renderers::Renderers renderer)
{
	if (renderer == Renderers::Renderer3D)
	{
		for (auto& meshRender : getComponents<MeshRender3D>())
		{
			for (auto& mesh : meshRender->getModel()->getMeshes())
				Renderer3D::submit(mesh.getGeometry(), meshRender->getMaterial(), meshRender->getModelMatrix());
		}
	}
}
