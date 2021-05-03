/*! \file settingsScript.cpp
*
* \brief A script to show the settings menu
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/settingsScript.h"
#include "independent/entities/entity.h"
#include "loaders/sceneLoader.h"

//! SettingsScript()
SettingsScript::SettingsScript()
{
}

//! ~SettingsScript()
SettingsScript::~SettingsScript()
{
}

//! onAttach()
void SettingsScript::onAttach()
{
	SceneLoader::loadEntities(getParent()->getParentScene(), "assets/scenes/common/settingsMenu.json");
}