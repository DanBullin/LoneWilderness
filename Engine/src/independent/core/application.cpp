/*! \file application.cpp
*
* \brief Main application, the start point of the engine
*
* \author DMU Course material
*
*/

#include "independent/core/application.h"
#include "independent/systems/systemManager.h"

namespace Engine {
	Application* Application::s_instance = nullptr; //!< Set the static instance pointer to null
	bool Application::s_running = true; //!< Set to true

	//! Application()
	Application::Application()
	{
		// Set instance variable
		if (s_instance == nullptr) { s_instance = this; }

		// Start all systems

		// Initialise with the maximum number of systems
		SystemManager::initialise(10);

		SystemManager::addSystem(Systems::Type::Logger);
		SystemManager::addSystem(Systems::Type::Randomiser);
		SystemManager::addSystem(Systems::Type::TimerSystem);
		SystemManager::addSystem(Systems::Type::WindowAPISystem);
		SystemManager::addSystem(Systems::Type::WindowManager);
		SystemManager::addSystem(Systems::Type::EventManager);
		SystemManager::addSystem(Systems::Type::ResourceManager);
		SystemManager::addSystem(Systems::Type::SceneManager);
		SystemManager::addSystem(Systems::Type::RenderSystem);
		SystemManager::addSystem(Systems::Type::FontManager);

		// Start timers for FPS and total application time
		TimerSystem::startTimer("FPS");
		TimerSystem::startTimer("TotalTime");
	}

	//! ~Application()
	Application::~Application()
	{
		s_instance = nullptr;

		// Destroy all systems and their resources
		SystemManager::destroy();
	}

	//! run()
	void Application::run()
	{
		while (s_running)
		{
			// Update all necassary items through the event manager
			TIME_FUNCTION("EventsUpdate", EventManager::onUpdate(GET_TIME("FPS", true, true), GET_TIME("TotalTime", false, true)));

			// If we have a valid focused window, render the current active scene
			if (WindowManager::getFocusedWindow())
				TIME_FUNCTION("Rendering", RenderSystem::onRender(SceneManager::getActiveScene().get()));

			if(SceneManager::getActiveScene()->getNewEntitiesFlag())
				SceneManager::getActiveScene()->setNewEntitiesFlag(false);

			// Destroy all scheduled scenes
			SceneManager::destroyScheduledScenes();

			// Deregister all windows that are scheduled to be deleted
			// This must be done at the end of the frame
			WindowManager::deregisterScheduledWindows();

			// No more windows are open, so let's close the application
			if (WindowManager::getRegisteredWindows().size() == 0)
				Application::stop();
		}
	}

	//! stop()
	void Application::stop()
	{
		// Stop the game loop
		s_running = false;
	}
}