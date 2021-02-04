/*! \file systemManager.h 
*
* \brief A system manager class which manages all systems that can be added to an application.
*
* \author Daniel Bullin
*
*/
#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "independent/systems/systems/log.h"
#include "independent/systems/systems/randomiser.h"
#include "independent/systems/systems/timerSystem.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/eventManager.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/renderSystem.h"
#include "independent/systems/systems/fontManager.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/systems/GLFWSystem.h"
#endif

namespace Engine
{
	/*! \class SystemManager
	* \brief A manager class which manages all systems
	*/
	class SystemManager
	{
	private:
		static bool s_enabled; //!< Has the system manager been enabled
		static std::vector<System*> s_activeSystems; //!< List of all active systems
	public:
		static void initialise(const uint32_t systemMaxCount); //!< Initialise the system manager

		static void addSystem(const Systems::Type type); //!< Add a system to the back of system list.
		static void destroy(); //!< Destroys the system manager and all systems
		static const bool containsSystem(const Systems::Type type); //!< Check if system type is enabled

		static std::vector<System*>& getActiveSystems(); //!< Get all active systems
		static System* getSystemByType(const Systems::Type type); //!< Get a system by type
		template<typename T> static T* getSystem(); //!< Get a system by pointer
	};

	template<typename T>
	//! getSystem()
	/*!
	\return a T* - A pointer to the system
	*/
	static T* SystemManager::getSystem()
	{
		// Check template type against system types
		Systems::Type type;

		switch (typeid(T))
		{
			case typeid(Log):
			{
				type = Systems::Type::Logger;
				break;
			}
			case typeid(Randomiser):
			{
				type = Systems::Type::Randomiser;
				break;
			}
			case typeid(TimerSystem):
			{
				type = Systems::Type::TimerSystem;
				break;
			}
			case typeid(WindowManager):
			{
				type = Systems::Type::WindowManager;
				break;
			}
			case typeid(EventManager):
			{
				type = Systems::Type::EventManager;
				break;
			}
			case typeid(ResourceManager):
			{
				type = Systems::Type::ResourceManager;
				break;
			}
			case typeid(SceneManager):
			{
				type = Systems::Type::SceneManager;
				break;
			}
			case typeid(RenderSystem):
			{
				type = Systems::Type::RenderSystem;
				break;
			}
			case typeid(FontManager):
			{
				type = Systems::Type::FontManager;
				break;
			}
			default:
			{
				return nullptr;
			}
		}

		// Loop through each item and check the system type
		for (auto& system : s_activeSystems)
			if (system->getSystemType() == type)
				return static_cast<T*>(system);

		// System by type not found
		return nullptr;
	}
}
#endif