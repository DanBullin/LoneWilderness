/*! \file engineScript.h
*
* \brief A script to use some engine features
*
* \author: Daniel Bullin
*
*/
#include "scripts/engineScript.h"
#include "settings/settings.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"

//! EngineScript()
EngineScript::EngineScript()
{
}

//! ~OwlEngine()
EngineScript::~EngineScript()
{
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void EngineScript::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::FullscreenWindow).first)
		WindowManager::getFocusedWindow()->setFullscreen(!WindowManager::getFocusedWindow()->getProperties().getFullScreen()); // Toggle Fullscreen

	if (e.getKeyCode() == Keys::B && InputPoller::isKeyPressed(Keys::LEFT_CONTROL))
	{
		ResourceManager::setConfigValue(Config::ApplyFog, !ResourceManager::getConfigValue(Config::ApplyFog));
	}
}