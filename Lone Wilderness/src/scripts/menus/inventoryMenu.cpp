/*! \file inventoryMenu.cpp
*
* \brief A script to show the inventory menu
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/inventoryMenu.h"
#include "settings/settings.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"
#include "loaders/sceneLoader.h"
#include "scripts/layerControl.h"
#include "scripts/gameObjects/player.h"
#include "scripts/gameObjects/player/inventory.h"

//! InventoryMenu()
InventoryMenu::InventoryMenu()
{
	m_updated = true;
	m_player = nullptr;
}

//! ~InventoryMenu()
InventoryMenu::~InventoryMenu()
{
}

//! onAttach()
void InventoryMenu::onAttach()
{
	SceneLoader::loadEntities(getParent()->getParentScene(), "assets/scenes/common/inventory.json");
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void InventoryMenu::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	Scene* scene = getParent()->getParentScene();

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Inventory).first)
	{
		if (LayerControl::isShow("Inven", getParent()->getParentScene()))
		{
			LayerControl::hide("Inven", scene);
			LayerControl::deactivate("Inven", scene);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
			SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(false);
			LayerControl::show("UI", scene);
		}
		else if(LayerControl::isShow("UI", getParent()->getParentScene()))
		{
			LayerControl::show("Inven", scene);
			LayerControl::activate("Inven", scene);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
			SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(true);
			LayerControl::hide("UI", scene);
		}
	}
}

void InventoryMenu::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_player)
	{
		m_player = static_cast<Player*>(SceneManager::getActiveScene()->getEntity("Player1")->getComponent<NativeScript>());
	}

	if (m_inventoryElements.size() == 0)
	{
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem0Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem1Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem2Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem3Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem4Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem5Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem6Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem7Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem8Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem9Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem10Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem11Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem12Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem13Image")->getComponent<MeshRender2D>());
		m_inventoryElements.push_back(SceneManager::getActiveScene()->getEntity("InvenItem14Image")->getComponent<MeshRender2D>());
	}
	else
	{
		// Update the inventory items based on player's inventory
		if (m_player)
		{
			for (int i = 0; i < INVENLIMIT; i++)
			{
				if (m_player->getInventory()->getItem(i))
				{
					m_inventoryElements.at(i)->getParent()->setDisplay(true);
					m_inventoryElements.at(i)->setMaterial(Items::getMaterial(m_player->getInventory()->getItem(i)->getType()));
				}
				else
				{
					m_inventoryElements.at(i)->getParent()->setDisplay(false);
				}
			}
		}
	}

}
