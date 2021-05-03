/*! \file resourcesScript.cpp
*
* \brief A resources script
*
* \author: Daniel Bullin
*
*/
#include "scripts/loading/resourcesScript.h"
#include "settings/settings.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/threadManager.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/utils/resourceLoader.h"
#include "loaders/sceneLoader.h"

//! ResourcesScript()
ResourcesScript::ResourcesScript()
{
	m_uploadedModels = false;
	m_loadingText = nullptr;
	m_thread = nullptr;
}

//! ~ResourcesScript()
ResourcesScript::~ResourcesScript()
{
}

//! onAttach()
void ResourcesScript::onAttach()
{
	Settings::loadFromFile();
	ResourceManager::loadTResources();
	ResourceLoader::uploadModels();
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void ResourcesScript::onPostUpdate(const float timestep, const float totalTime)
{
	if (!m_loadingText || !m_thread)
	{
		m_loadingText = getParent()->getParentScene()->getEntity("LoadingText")->getComponent<Text>();
		m_thread = ThreadManager::getThread("LoadResources");
	}

	m_loadingText->setText("Loading Resource: " + ResourceManager::getResBeingLoaded());


	if (totalTime > 5.f)
	{
		SceneLoader::load("mainMenu", "assets/scenes/mainMenu/");
		SceneManager::setActiveScene("mainMenu", false);
	}
}