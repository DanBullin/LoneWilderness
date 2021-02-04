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
	Map<std::string, Scene> SceneManager::s_scenesList = Map<std::string, Scene>(); //!< Initialise empty list
	std::string SceneManager::s_activeSceneName = std::string(); //!< Initialise to empty string

	//! SceneManager()
	SceneManager::SceneManager() : System(Systems::Type::SceneManager)
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
			destroyScenes();
			s_enabled = false;
		}
	}

	//! createScene()
	/*!
	\param sceneName a const char* - The name of the scene to load
	\return a Scene* - A pointer to the new scene
	*/
	Scene* SceneManager::createScene(const char* sceneName)
	{
		if (s_enabled)
		{
			// Check if the scene name is already taken
			if (s_scenesList.find(sceneName) == s_scenesList.end())
			{
				// Scene name is free, so load the scene
				Shared<Scene> newScene;
				newScene.reset(new Scene(sceneName));

				s_scenesList[sceneName] = newScene;
				ResourceManager::loadResourcesByScene(sceneName);

				// If this is the first scene to be loaded, set it as the active scene
				if (s_scenesList.size() == 1)
					s_activeSceneName = sceneName;

				return newScene.get();
			}
			else
				ENGINE_ERROR("[SceneManager::loadScene] Scene name already exists, cannot load the new scene. Name: {0}", sceneName);

			// If no scenes exist after an attempt to load a scene, set a default scene
			if (s_scenesList.size() == 0)
				return createScene("default");
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
			// Find the scene and erase it
			if (s_scenesList.find(sceneName) != s_scenesList.end())
			{
				// Only if the scene to be deleted is NOT the current active scene
				if (sceneName != s_activeSceneName)
					s_scenesList[sceneName]->setDelete(true);
				else
					ENGINE_ERROR("[SceneManager::deleteScene] Cannot destroy scene as it is currently set as the active scene. Name: {0}.", sceneName);
			}
			else
				ENGINE_ERROR("[SceneManager::deleteScene] Cannot destroy scene as name cannot be found. Name: {0}", sceneName);

			// If no scenes are left, load a default scene
			if (s_scenesList.size() == 0)
				createScene("default");
		}
		else
			ENGINE_ERROR("[SceneManager::destroyScene] This system has not been enabled.");
	}

	//! destroyScenes()
	void SceneManager::destroyScenes()
	{
		if (s_enabled)
		{
			// Clear all scenes
			for (auto& scene : s_scenesList)
			{
				scene.second->destroy();
				scene.second.reset();
				scene.second = nullptr;
			}

			s_scenesList.clear();
			s_activeSceneName = "NULL";
		}
		else
			ENGINE_ERROR("[SceneManager::destroyScenes] This system has not been enabled.");
	}

	//! destroyScheduledScenes()
	void SceneManager::destroyScheduledScenes()
	{
		if (s_enabled)
		{
			for (auto it = s_scenesList.cbegin(); it != s_scenesList.cend() ; )
			{
				if (it->second->getDelete())
				{
					it->second->destroy();
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
	\return a std::shared_ptr<Scene> - A pointer to the scene
	*/
	Shared<Scene> SceneManager::getScene(const char* sceneName)
	{
		if (s_enabled)
		{
			// Find the scene by name
			if (s_scenesList.find(sceneName) != s_scenesList.end())
				return s_scenesList[sceneName];
		}
		else
			ENGINE_ERROR("[SceneManager::getScene] This system has not been enabled.");

		return nullptr;
	}

	//! getSceneList()
	/*!
	\return a std::map<std::string, std::shared_ptr<Scene>>& - A reference to the list of scenes
	*/
	Map<std::string, Scene>& SceneManager::getSceneList()
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
	\return a std::shared_ptr<Scene> - A pointer to the active scene
	*/
	Shared<Scene> SceneManager::getActiveScene()
	{
		Shared<Scene> scene = nullptr;

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
			if (s_scenesList.find(sceneName) != s_scenesList.end())
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
			ENGINE_TRACE("Active Scene: {0}", (void*)getActiveScene().get());
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