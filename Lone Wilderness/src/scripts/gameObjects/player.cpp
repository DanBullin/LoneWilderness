/*! \file player.cpp
*
* \brief A player dummy class
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/player.h"
#include "loaders/sceneLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/eventManager.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/utils/mathUtils.h"
#include "scripts/gameObjects/terrain.h"
#include "settings/settings.h"

//! Player()
Player::Player()
{
	m_controller = nullptr;
	m_healthText = new Text("0", { 1.0, 1.0, 1.0, 1.0 }, "Ubuntu20");
	m_dmgText = new Text("0", { 1.0, 1.0, 1.0, 1.0 }, "Ubuntu20");
	m_health = 100;
	m_atkDmg = 25;
}

//! ~Player()
Player::~Player()
{
	if (m_hotbar) delete m_hotbar;
	m_hotbar = nullptr;

	if (m_inventory) delete m_inventory;
	m_inventory = nullptr;
}


void Player::onAttach()
{
	m_controller = getParent()->getComponent<CharacterController>();
	m_hotbar = new Hotbar;
	m_inventory = new Inventory;

	m_inventory->onAttach();
}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onPreUpdate(const float timestep, const float totalTime)
{
	m_hotbar->onPreUpdate(timestep, totalTime);
	m_inventory->onPreUpdate(timestep, totalTime);

	Transform* trans = getParent()->getComponent<Transform>();
	Terrain* terrain = static_cast<Terrain*>(getParent()->getParentScene()->getEntity("Terrain1")->getComponent<NativeScript>());
	float y = terrain->getYCoord(trans->getWorldPosition().x, trans->getWorldPosition().z);
	trans->setLocalPosition({ trans->getWorldPosition().x, y , trans->getWorldPosition().z });
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
	m_hotbar->onKeyPress(e, timestep, totalTime);

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveForward).first)
	{
		if (m_controller)
			m_controller->move(FORWARD, timestep);
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveLeft).first)
	{
		if (m_controller)
			m_controller->move(LEFT, timestep);
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveBack).first)
	{
		if (m_controller)
			m_controller->move(BACKWARD, timestep);
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveRight).first)
	{
		if (m_controller)
			m_controller->move(RIGHT, timestep);
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
	if (e.getKeyCode() == Keys::J)
	{
		m_inventory->print();
	}

	if (e.getKeyCode() == Keys::K)
	{
		m_inventory->giveItem(Items::Log, 0, 7);
		m_inventory->giveItem(Items::Axe, 1, 12);
		m_inventory->giveItem(Items::Axe, 2, 3);
		m_inventory->giveItem(Items::Axe, 3, 3);
		m_inventory->giveItem(Items::Axe, 4, 3);
	}

	if (e.getKeyCode() == Keys::L)
	{
		m_inventory->takeItem(Items::Log, 0, 7);
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
	m_hotbar->onMouseScrolled(e, timestep, totalTime);
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

void Player::onRender(const Renderers renderer, const std::string & renderState)
{
	if (renderer == Renderers::Renderer2D)
	{
		glm::vec4 healthColour = { 1.f, 1.f, 1.f, 1.f };
		glm::vec4 atkDmgColour = { 1.f, 1.f, 1.f, 1.f };

		if (m_health > 75)
			healthColour = { 0.f, 1.f, 0.f, 1.f };
		else if(m_health > 50)
			healthColour = { 1.f, 0.5f, 0.1f, 1.f };
		else if (m_health > 25)
			healthColour = { 1.f, 1.f, 0.f, 1.f };
		else if (m_health >= 0)
			healthColour = { 1.f, 0.f, 0.f, 1.f };

		m_healthText->setColour(healthColour);
		m_healthText->setText(std::to_string(m_health));

		m_dmgText->setColour(atkDmgColour);
		m_dmgText->setText(std::to_string(m_atkDmg));

		auto size = WindowManager::getFocusedWindow()->getProperties().getSizef();
		Renderer2D::submitText(m_healthText, MathUtils::getModelMatrix({ (size.x - 36), 26.f, 0.5f }));
		Renderer2D::submitText(m_dmgText, MathUtils::getModelMatrix({ (size.x - 36), 59.f, 0.5f }));
	}
}

uint32_t Player::getHealth()
{
	return m_health;
}

uint32_t Player::getAttackDamage()
{
	return m_atkDmg;
}

Hotbar * Player::getHotbar()
{
	return m_hotbar;
}

Inventory * Player::getInventory()
{
	return m_inventory;
}