/*! \file pauseScript.cpp
*
* \brief A script to show the pause menu
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/pauseScript.h"
#include "settings/settings.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"
#include "loaders/sceneLoader.h"
#include "scripts/layerControl.h"

//! PauseScript()
PauseScript::PauseScript()
{
}

//! ~PauseScript()
PauseScript::~PauseScript()
{
}

//! onAttach()
void PauseScript::onAttach()
{
	SceneLoader::loadEntities(getParent()->getParentScene(), "assets/scenes/common/pauseMenu.json");
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void PauseScript::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	Scene* scene = getParent()->getParentScene();

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Pause).first)
	{
		LayerControl::show("Pause", scene);
		LayerControl::activate("Pause", scene);
		WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
		SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(true);
		LayerControl::hide("UI", scene);
	}
}