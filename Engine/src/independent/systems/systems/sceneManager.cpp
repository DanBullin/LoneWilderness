/*! \file sceneManager.cpp
*
* \brief A scene manager which manages scenes
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	bool SceneManager::s_enabled = false; //!< Initialise with default value of false
	std::map<std::string, Scene*> SceneManager::s_scenesList = std::map<std::string, Scene*>(); //!< Initialise empty list
	std::string SceneManager::s_activeSceneName = std::string(); //!< Initialise to empty string

	//! SceneManager()
	SceneManager::SceneManager() : System(SystemType::SceneManager)
	{
	}

	//! ~SceneManager()
	SceneManager::~SceneManager()
	{
	}

	//! start()
	void SceneManager::start()
	{
		// Start this system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[SceneManager::start] Starting the scene manager system.");
			s_enabled = true;
		}
	}

	//! stop()
	void SceneManager::stop()
	{
		// Stop this system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[SceneManager::stop] Stopping the scene manager system.");
			destroyScene();
			s_enabled = false;
		}
	}

	//! sceneExists()
	/*!
	\param sceneName a const std::string& - The name of the scene to check
	\return a bool - Was the scene name found in the scene list
	*/
	bool SceneManager::sceneExists(const std::string& sceneName)
	{
		return s_scenesList.find(sceneName) != s_scenesList.end();
	}

	//! createScene()
	/*!
	\param sceneName a const std::string& - The name of the scene to load
	\return a Scene* - A pointer to the new scene
	*/
	Scene* SceneManager::createScene(const std::string& sceneName)
	{
		if (s_enabled)
		{
			// Check if the scene name is already taken
			if (!sceneExists(sceneName))
			{
				// Scene name is free, so load the scene
				Scene* newScene = new Scene(sceneName.c_str());
				s_scenesList[sceneName] = newScene;
	
				// If this is the first scene to be loaded, set it as the active scene
				if (s_scenesList.size() == 1)
					s_activeSceneName = sceneName;

				return newScene;
			}
			else
				ENGINE_ERROR("[SceneManager::loadScene] Scene name already exists, cannot load the new scene. Name: {0}", sceneName);
		}
		else
			ENGINE_ERROR("[SceneManager::loadScene] This system has not been enabled.");

		return nullptr;
	}

	//! destroyScene()
	/*!
	\param sceneName a const std::string& - The name of the scene to destroy
	*/
	void SceneManager::destroyScene(const std::string& sceneName)
	{
		if (s_enabled)
		{
			if (sceneName != "")
			{
				// Find the scene and erase it
				if (sceneExists(sceneName))
				{
					// Only if the scene to be deleted is NOT the current active scene
					if (sceneName != s_activeSceneName || s_scenesList.size() != 1)
						s_scenesList[sceneName]->destroy();
					else
						ENGINE_ERROR("[SceneManager::deleteScene] Cannot destroy scene as it is currently set as the active scene or is the last scene available. Name: {0}.", sceneName);
				}
				else
					ENGINE_ERROR("[SceneManager::deleteScene] Cannot destroy scene as name cannot be found. Name: {0}", sceneName);
			}
			else
			{
				// Name is blank, delete all scenes
				for (auto& scene : s_scenesList)
				{
					delete scene.second;
					scene.second = nullptr;
				}

				s_scenesList.clear();
				s_activeSceneName = "NULL";
			}
		}
		else
			ENGINE_ERROR("[SceneManager::destroyScene] This system has not been enabled.");
	}

	//! destroyScheduledScenes()
	void SceneManager::destroyScheduledScenes()
	{
		if (s_enabled)
		{
			for (auto it = s_scenesList.cbegin(); it != s_scenesList.cend(); )
			{
				if (it->second->getDestroyed())
				{
					delete it->second;
					s_scenesList.erase(it++);
				}
				else
					++it;
			}
		}
		else
			ENGINE_ERROR("[SceneManager::destroyScheduledScenes] This system has not been enabled.");
	}

	//! getScene()
	/*!
	\param sceneName a const char* - The name of the scene to get
	\return a Scene* - A pointer to the scene
	*/
	Scene* SceneManager::getScene(const char* sceneName)
	{
		if (s_enabled)
		{
			// Find the scene by name
			if (sceneExists(sceneName))
				return s_scenesList[sceneName];
		}
		else
			ENGINE_ERROR("[SceneManager::getScene] This system has not been enabled.");

		return nullptr;
	}

	//! getSceneList()
	/*!
	\return a std::map<std::string, Scene*>& - A reference to the list of scenes
	*/
	std::map<std::string, Scene*>& SceneManager::getSceneList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[SceneManager::getSceneList] This system has not been enabled.");

		// Check if scene list is empty
		if (s_scenesList.size() == 0)
			ENGINE_ERROR("[SceneManager::getScenes] There are no scenes in the scene list.");

		return s_scenesList;
	}

	//! getActiveScene()
	/*!
	\return a Scene* - A pointer to the active scene
	*/
	Scene* SceneManager::getActiveScene()
	{
		Scene* scene = nullptr;

		if (s_enabled)
			scene = getScene(s_activeSceneName.c_str());
		else
			ENGINE_ERROR("[SceneManager::getActiveScene] This system has not been enabled.");
		return scene;
	}

	//! setActiveScene()
	/*!
	\param sceneName a const char* - The name of the scene to set as active
	\param deleteOldScene a const bool - Delete the old active scene
	*/
	void SceneManager::setActiveScene(const char* sceneName, const bool deleteOldScene)
	{
		if (s_enabled)
		{
			// Check if the scene to set as active exists
			if (sceneExists(sceneName))
			{
				//	Check to make sure the we're not setting the active scene as the current active scene
				if (s_activeSceneName != sceneName)
				{
					std::string oldSceneName = s_activeSceneName;
					s_activeSceneName = sceneName;

					// Destroy the old active scene if needed
					if (deleteOldScene)
						destroyScene(oldSceneName.c_str());
				}
				else
					ENGINE_ERROR("[SceneManager::setActiveScene] This scene is already set as the active scene. Name: {0}", sceneName);
			}
			else
				ENGINE_ERROR("[SceneManager::setActiveScene] Could not find the scene to set as active. Name: {0}", sceneName);
		}
		else
			ENGINE_ERROR("[SceneManager::setActiveScene] This system has not been enabled.");
	}

	//! printSceneManagerDetails()
	void SceneManager::printSceneManagerDetails()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("Scene Manager Details");
			ENGINE_TRACE("==========================");
			ENGINE_TRACE("Scene List Size: {0}", s_scenesList.size());
			ENGINE_TRACE("Active Scene Name: {0}", s_activeSceneName);
			ENGINE_TRACE("Active Scene: {0}", (void*)getActiveScene());
			ENGINE_TRACE("==========================");
		}
		else
			ENGINE_ERROR("[SceneManager::printSceneManagerDetails] This system has not been enabled.");
	}

	//! printScenesDetails()
	void SceneManager::printScenesDetails()
	{
		if (s_enabled)
		{
			for (auto& scene : s_scenesList)
				scene.second->printSceneDetails();
		}
		else
			ENGINE_ERROR("[SceneManager::printScenesDetails] This system has not been enabled.");
	}
}