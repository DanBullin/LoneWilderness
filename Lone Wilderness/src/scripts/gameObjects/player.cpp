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
}


void Player::onAttach()
{
	m_controller = getParent()->getComponent<CharacterController>();
	m_hotbar = new Hotbar;
}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onPreUpdate(const float timestep, const float totalTime)
{
	m_hotbar->onPreUpdate(timestep, totalTime);

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

	if (e.getKeyCode() == Keys::U)
	{
		SceneManager::getActiveScene()->getEntity("NewObj0")->setSelected(true);
	}

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
		ResourceManager::setConfigValue(Config::ApplyFog, !ResourceManager::getConfigValue(Config::ApplyFog));
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
