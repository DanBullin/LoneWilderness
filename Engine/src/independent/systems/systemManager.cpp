/*! \file systemManager.cpp
*
* \brief A system manager class which manages all systems that can be added to an application.
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systemManager.h"

namespace Engine
{
	std::vector<System*> SystemManager::s_activeSystems = std::vector<System*>(); //!< Initialise static variable
	bool SystemManager::s_enabled = false; //!< Initialise starting as false

	//! initialise()
	/*!
	\param systemMaxCount a const uint32_t - The max number of systems
	*/
	void SystemManager::initialise(const uint32_t systemMaxCount)
	{
		// Initialise the SM if it hasn't been already
		if(!s_enabled)
		{
			if (s_activeSystems.size() != 0)
				s_activeSystems.clear();

			s_activeSystems.reserve(systemMaxCount);
			s_enabled = true;
		}
	}

	//! destroy()
	void SystemManager::destroy()
	{
		// Destroy the SystemManager if it has been initialised
		if (s_enabled)
		{
			// Check if list is empty
			if (s_activeSystems.size() > 0)
			{
				// Go through the list in reverse
				for (std::vector<System*>::reverse_iterator i = s_activeSystems.rbegin(); i != s_activeSystems.rend(); ++i)
				{
					// Stop the system
					if ((*i))
					{
						(*i)->stop();
						delete (*i);
					}
					else
						ENGINE_ERROR("[SystemManager::destroy] System is a null pointer.");
				}
				// All systems have been stopped, clear the list
				s_activeSystems.clear();
			}
			else
				ENGINE_ERROR("[SystemManager::destroy] Cannot remove systems as system list is empty.");

			s_enabled = false;
		}
		else
			ENGINE_ERROR("[SystemManager::destroy] The system manager has not been enabled.");

	}

	//! containsSystem()
	/*!
	\param type a const Systems::Type - The system type
	\return a const bool - Is the system enabled
	*/
	const bool SystemManager::containsSystem(const Systems::Type type)
	{
		if (s_enabled)
		{
			// Loop through each item and check the system type
			for (auto& system : s_activeSystems)
				if (system->getSystemType() == type)
					return true;
		}
		return false;
	}

	//! addSystem()
	/*!
	\param type a const Systems::Type - The system type
	*/
	void SystemManager::addSystem(const Systems::Type type)
	{
		// Add system if manager is enabled
		if (s_enabled)
		{
			// Check the current number of systems is less than the max capacity
			if (s_activeSystems.size() < s_activeSystems.capacity())
			{
				// Capacity has not been reached, check if system type has been added
				if (!containsSystem(type))
				{
					// Add the system
					switch (type)
					{
						case Systems::Type::Logger:
						{
							s_activeSystems.push_back(new Log);
							break;
						}
						case Systems::Type::Randomiser:
						{
							s_activeSystems.push_back(new Randomiser);
							break;
						}
						case Systems::Type::TimerSystem:
						{
							s_activeSystems.push_back(new TimerSystem);
							break;
						}
						case Systems::Type::WindowAPISystem:
						{
#ifdef NG_PLATFORM_WINDOWS
							s_activeSystems.push_back(new GLFWSystem);
#endif
							break;
						}
						case Systems::Type::WindowManager:
						{
							// Check if window API has been added, cannot add window manager if it hasn't
							if (containsSystem(Systems::Type::WindowAPISystem))
								s_activeSystems.push_back(new WindowManager);
							else
							{
								ENGINE_ERROR("[SystemManager::addSystem] Cannot add WindowManager system until Windowing API system has been added.");
								return;
							}
							break;
						}
						case Systems::Type::EventManager:
						{
							s_activeSystems.push_back(new EventManager);
							break;
						}
						case Systems::Type::ResourceManager:
						{
							s_activeSystems.push_back(new ResourceManager);
							break;
						}
						case Systems::Type::SceneManager:
						{
							s_activeSystems.push_back(new SceneManager);
							break;
						}
						case Systems::Type::RenderSystem:
						{
							s_activeSystems.push_back(new RenderSystem);
							break;
						}
						case Systems::Type::FontManager:
						{
							s_activeSystems.push_back(new FontManager);
							break;
						}
						default:
							break;
					}

					// System has been added, start the system
					getSystemByType(type)->start();
				}
				else
					ENGINE_ERROR("[SystemManager::addSystem] Cannot add system as system type already exists: {0}", type);
			}
			else
				ENGINE_ERROR("[SystemManager::addSystem] Cannot add system has the maximum number of systems have been added. System Type: {0}", type);
		}
		else
			ENGINE_ERROR("[SystemManager::addSystem] The system manager has not been enabled.");
	}

	//! getActiveSystems()
	/*!
	\return a std::vector<System*>& - The active systems list
	*/
	std::vector<System*>& SystemManager::getActiveSystems()
	{
		if(!s_enabled)
			ENGINE_ERROR("[SystemManager::getActiveSystems] The system manager has not been enabled.");
		return s_activeSystems;
	}

	//! getSystemByType()
	/*!
	\param type a const Systems::Type - The system type
	\return a System* - The system pointer
	*/
	System* SystemManager::getSystemByType(const Systems::Type type)
	{
		if (s_enabled)
		{
			// Loop through each item and check the system type
			for (auto& system : s_activeSystems)
				if (system->getSystemType() == type)
					return system;
		}
		else
			ENGINE_ERROR("[SystemManager::getSystemByType] The system manager has not been enabled.");
		return nullptr;
	}
}