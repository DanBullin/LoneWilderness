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
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/fontManager.h"

//! Player()
Player::Player()
{
	m_controller = nullptr;
}

//! ~Player()
Player::~Player()
{
}

void Player::onAttach()
{
	m_controller = getParent()->getComponent<CharacterController>();
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
		Entity* ent = new Entity;

		// TEST CRASHINGGGG
		if (getParent()->getParentScene()->getEntity("Cyborg1")->addChildEntity("Child1", ent))
		{
			ent->attach<Transform>("Transform1", 2.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f);
			ent->setLayer(getParent()->getParentScene()->getLayerManager()->getLayer("Default"));
			ent->setDisplay(true);
			ent->attach<MeshRender3D>("MeshRender3D", ResourceManager::getResource<Model3D>("cyborg"), nullptr);

			Entity* ent1 = new Entity;
			if (ent->addChildEntity("Child2", ent1))
			{
				ent1->attach<Transform>("Transform1", 5.f, 0.f, 0.f, 0.f, 45.f, 0.f, 1.f, 1.f, 1.f);
				ent1->setLayer(getParent()->getParentScene()->getLayerManager()->getLayer("Default"));
				ent1->setDisplay(true);
				ent1->attach<MeshRender3D>("MeshRender3D", ResourceManager::getResource<Model3D>("cyborg"), nullptr);

				Entity* ent2 = new Entity;
				if (ent1->addChildEntity("Child3", ent2))
				{
					ent2->attach<Transform>("Transform1", 5.f, 0.f, 0.f, 0.f, 45.f, 0.f, 1.f, 1.f, 1.f);
					ent2->setLayer(getParent()->getParentScene()->getLayerManager()->getLayer("Default"));
					ent2->setDisplay(true);
					ent2->attach<MeshRender3D>("MeshRender3D", ResourceManager::getResource<Model3D>("cyborg"), nullptr);
				}
				else
					delete ent1;

			}
			else
				delete ent1;

		}
		else
			delete ent;
	}

	if (e.getKeyCode() == Keys::N)
	{
		getParent()->getChildEntity("Child1")->destroy();
	}

	if (e.getKeyCode() == Keys::M)
	{
		auto t = getParent()->getChildEntities();

		ENGINE_INFO("h");

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