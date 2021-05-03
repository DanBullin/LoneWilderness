/*! \file hotbar.h
*
* \brief The player's hotbar
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/player/hotbar.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/SceneManager.h"
#include "settings/settings.h"

Hotbar::Hotbar()
{
	m_currentItem = 0;
	m_currentElement = nullptr;
}

Hotbar::~Hotbar()
{
}

void Hotbar::onAttach()
{
}

void Hotbar::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_currentElement)
	{
		m_currentElement = SceneManager::getActiveScene()->getEntity("hotbarCurrent")->getComponent<UIElement>();
	}
	else
	{
		m_currentElement->setOffset({ m_currentElement->getOffset().x, -132.f + (66.f * m_currentItem) });
	}
}

void Hotbar::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar1).first)
		m_currentItem = 0;
	else if(e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar2).first)
		m_currentItem = 1;
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar3).first)
		m_currentItem = 2;
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar4).first)
		m_currentItem = 3;
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar5).first)
		m_currentItem = 4;
}

void Hotbar::onMouseScrolled(MouseScrolledEvent & e, const float timestep, const float totalTime)
{
	if (e.getYOffset() == 1)
		scrollUp();
	else
		scrollDown();
}

void Hotbar::setHotbarItem(const uint32_t hotbarIndex, const uint32_t item)
{
}

void Hotbar::scrollDown()
{
	if (m_currentItem < s_maxCount-1)
		m_currentItem++;
}

void Hotbar::scrollUp()
{
	if (m_currentItem > 0)
		m_currentItem--;
}