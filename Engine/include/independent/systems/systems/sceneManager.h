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
		static Map<std::string, Scene> s_scenesList; //!< List of all the loaded scenes
		static std::string s_activeSceneName; //!< The active scene name
	public:
		SceneManager(); //!< Constructor
		~SceneManager(); //!< Destructor

		virtual void start() override; //!< Start the system
		virtual void stop() override; //!< Stop the system

		static Scene* createScene(const char* sceneName); //!< Load the scene
		static void destroyScene(const std::string& sceneName); //!< Destory a scene
		static void destroyScenes(); //!< Destory all scenes
		static void destroyScheduledScenes(); //!< Destroy all scheduled scenes

		static Shared<Scene> getScene(const char* sceneName); //!< Get a scene by name
		static Map <std::string, Scene>& getSceneList(); //!< Get a list of all the scenes

		static Shared<Scene> getActiveScene(); //!< Get the active scene
		static void setActiveScene(const char* sceneName, const bool deleteOldScene); //!< Set the active scene

		static void printSceneManagerDetails(); //!< Print scene manager details
		static void printScenesDetails(); //!< Print scenes details
	};
}

#endif