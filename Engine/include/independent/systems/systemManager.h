/*! \file systemManager.h 
*
* \brief A system manager class which manages all systems that have been loaded.
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
#include "independent/systems/systems/fontManager.h"
#include "independent/systems/systems/renderSystem.h"
#include "independent/systems/systems/threadManager.h"

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

		static const bool containsSystem(const SystemType type); //!< Check if system type has been added to the system list
		static void addSystem(const SystemType type); //!< Add a system to the back of system list.
		static void destroy(); //!< Destroys the system manager and all systems

		static System* getSystemByType(const SystemType type); //!< Get a system by type
	};

}
#endif