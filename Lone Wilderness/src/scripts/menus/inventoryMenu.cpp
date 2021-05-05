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
#include "scripts/menus/inventoryElement.h"

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

			for (int i = 0; i < INVENLIMIT; i++)
			{
				InventoryElement* element = static_cast<InventoryElement*>(getParent()->getParentScene()->getEntity("InvenItem" + std::to_string(i) + "Image")->getComponent<NativeScript>());
				if (element)
				{
					element->resetSlot();
				}
			}
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
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem0Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem0Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem1Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem1Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem2Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem2Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem3Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem3Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem4Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem4Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem5Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem5Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem6Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem6Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem7Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem7Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem8Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem8Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem9Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem9Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem10Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem10Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem11Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem11Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem12Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem12Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem13Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem13Text")->getComponent<Text>() });
		m_inventoryElements.push_back({ SceneManager::getActiveScene()->getEntity("InvenItem14Image")->getComponent<MeshRender2D>(), SceneManager::getActiveScene()->getEntity("InvenItem14Text")->getComponent<Text>() });
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
					m_inventoryElements.at(i).first->getParent()->setDisplay(true);
					m_inventoryElements.at(i).second->getParent()->setDisplay(true);
					m_inventoryElements.at(i).first->setMaterial(Items::getMaterial(m_player->getInventory()->getItem(i)->getType()));
					m_inventoryElements.at(i).second->setText(std::to_string(m_player->getInventory()->getItemCount(i)));
				}
				else
				{
					m_inventoryElements.at(i).first->getParent()->setDisplay(false);
					m_inventoryElements.at(i).second->getParent()->setDisplay(false);
				}
			}
		}
	}

	InventoryElement* slot1 = nullptr;
	InventoryElement* slot2 = nullptr;
	for (int i = 0; i < INVENLIMIT; i++)
	{
		InventoryElement* element = static_cast<InventoryElement*>(getParent()->getParentScene()->getEntity("InvenItem" + std::to_string(i) + "Image")->getComponent<NativeScript>());
		if (element)
		{
			if (element->getSlot())
			{
				if (!slot1)
				{
					slot1 = element;
					continue;
				}
				if (!slot2)
				{
					slot2 = element;
					continue;
				}
			}
		}
	}

	if (slot1 && slot2)
	{
		// Swap
		m_player->getInventory()->swapItems(slot1->getInvenIndex(), slot2->getInvenIndex());
		slot1->resetSlot();
		slot2->resetSlot();
	}

}
