/*! \file craftingMenu.cpp
*
* \brief A script to show the crafting menu
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/craftingMenu.h"
#include "settings/settings.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"
#include "loaders/sceneLoader.h"
#include "scripts/layerControl.h"

CraftingMenu::CraftingMenu()
{
	m_itemSelected = Items::None;
}

CraftingMenu::~CraftingMenu()
{
}

void CraftingMenu::onAttach()
{
	SceneLoader::loadEntities(getParent()->getParentScene(), "assets/scenes/common/crafting.json");
}

void CraftingMenu::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	Scene* scene = getParent()->getParentScene();

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Crafting).first)
	{
		if (LayerControl::isShow("Crafting", getParent()->getParentScene()))
		{
			LayerControl::hide("Crafting", scene);
			LayerControl::deactivate("Crafting", scene);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
			SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(false);
			LayerControl::show("UI", scene);
		}
		else if (LayerControl::isShow("UI", getParent()->getParentScene()))
		{
			LayerControl::show("Crafting", scene);
			LayerControl::activate("Crafting", scene);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
			SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(true);
			LayerControl::hide("UI", scene);
		}
	}
}

void CraftingMenu::onPreUpdate(const float timestep, const float totalTime)
{
}

void CraftingMenu::selectItem(Items::Items item)
{
	m_itemSelected = item;
}

Items::Items CraftingMenu::getSelectedItem()
{
	return m_itemSelected;
}
