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
#include "scripts/gameObjects/player.h"

Hotbar::Hotbar()
{
	m_currentItem = 0;
	m_currentElement = nullptr;
	m_updated = true;
	m_player = nullptr;
}

Hotbar::~Hotbar()
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
		if (m_updated)
		{
			m_currentElement->setOffset({ m_currentElement->getOffset().x, -132.f + (66.f * m_currentItem) });
			m_updated = false;
		}
	}

	if (!m_player)
	{
		m_player = static_cast<Player*>(SceneManager::getActiveScene()->getEntity("Player1")->getComponent<NativeScript>());
	}

	if (m_hotbarElements.size() == 0)
	{
		m_hotbarElements.push_back(SceneManager::getActiveScene()->getEntity("Item0Image")->getComponent<MeshRender2D>());
		m_hotbarElements.push_back(SceneManager::getActiveScene()->getEntity("Item1Image")->getComponent<MeshRender2D>());
		m_hotbarElements.push_back(SceneManager::getActiveScene()->getEntity("Item2Image")->getComponent<MeshRender2D>());
		m_hotbarElements.push_back(SceneManager::getActiveScene()->getEntity("Item3Image")->getComponent<MeshRender2D>());
		m_hotbarElements.push_back(SceneManager::getActiveScene()->getEntity("Item4Image")->getComponent<MeshRender2D>());
	}
	else
	{
		// Update the hotbar items based on player's inventory
		if (m_player)
		{
			for (int i = 0; i < s_maxCount; i++)
			{
				if (m_player->getInventory()->getItem(i))
				{
					m_hotbarElements.at(i)->getParent()->setDisplay(true);
					m_hotbarElements.at(i)->setMaterial(Items::getMaterial(m_player->getInventory()->getItem(i)->getType()));
				}
				else
				{
					m_hotbarElements.at(i)->getParent()->setDisplay(false);
				}
			}
		}
	}

}

void Hotbar::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar1).first)
		selectHotbar(0);
	else if(e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar2).first)
		selectHotbar(1);
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar3).first)
		selectHotbar(2);
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar4).first)
		selectHotbar(3);
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar5).first)
		selectHotbar(4);
}

void Hotbar::onMouseScrolled(MouseScrolledEvent & e, const float timestep, const float totalTime)
{
	if (e.getYOffset() == 1)
		scrollUp();
	else
		scrollDown();

	m_updated = true;
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

void Hotbar::selectHotbar(const uint32_t index)
{
	if (index >= s_maxCount) return;
	m_currentItem = index;
	m_updated = true;
}

uint32_t Hotbar::getSelectedItem()
{
	return m_currentItem;
}

uint32_t Hotbar::getMaxCount()
{
	return s_maxCount;
}