/*! \file sceneManager.h
*
* \brief A scene manager which manages scenes
*
* \author Daniel Bullin
*
*/
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "independent/core/common.h"
#include "independent/systems/system.h"
#include "independent/systems/components/scene.h"

namespace Engine
{
	/*! \class SceneManager
	* \brief A manager class which manages scenes
	*/
	class SceneManager : public System
	{
	private:
		static bool s_enabled; //!< Is the scene manager enabled
		static std::map<std::string, Scene*> s_scenesList; //!< List of all the loaded scenes
		static std::string s_activeSceneName; //!< The active scene name
	public:
		SceneManager(); //!< Constructor
		~SceneManager(); //!< Destructor

		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static bool sceneExists(const std::string& sceneName); //!< Check whether scene name exists
		static Scene* createScene(const std::string& sceneName, const std::string& sceneFolderPath); //!< Create a new scene
		static void destroyScene(const std::string& sceneName = ""); //!< Destory a scene
		static void destroyScheduledScenes(); //!< Destroy all scheduled scenes

		static Scene* getScene(const std::string& sceneName); //!< Get a scene by name
		static std::map<std::string, Scene*>& getSceneList(); //!< Get a list of all the scenes

		static Scene* getActiveScene(); //!< Get the active scene
		static const std::string& getActiveSceneName(); //!< Get the active scene name held in this class
		static void setActiveScene(const std::string& sceneName, const bool deleteOldScene); //!< Set the active scene

		static void printSceneManagerDetails(); //!< Print scene manager details
		static void printScenesDetails(); //!< Print scenes details
	};
}

#endif