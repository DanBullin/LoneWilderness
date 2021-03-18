/*! \file systemManager.cpp
*
* \brief A system manager class which manages all systems that have been loaded.
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systemManager.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/systems/GLFWSystem.h"
#endif

namespace Engine
{
	std::vector<System*> SystemManager::s_activeSystems; //!< Initialise static variable
	bool SystemManager::s_enabled = false; //!< Initialise starting as false

	//! initialise()
	/*!
	\param systemMaxCount a const uint32_t - The max number of systems
	*/
	void SystemManager::initialise(const uint32_t systemMaxCount)
	{
		// Initialise the System manager if it hasn't been already
		if(!s_enabled)
		{
			if (s_activeSystems.size() != 0)
			{
				for (auto& system : s_activeSystems)
				{
					if(system)
						delete system;
				}

				s_activeSystems.clear();
			}

			s_activeSystems.reserve(systemMaxCount);
			s_enabled = true;
		}
	}

	//! containsSystem()
	/*!
	\param type a const SystemType - The system type
	\return a const bool - Has the system been added to the system list
	*/
	const bool SystemManager::containsSystem(const SystemType type)
	{
		if (s_enabled)
		{
			// Loop through each item and check the system type
			for (auto& system : s_activeSystems)
			{
				if (system)
				{
					if (system->getSystemType() == type)
						return true;
				}
			}
		}
		return false;
	}

	//! addSystem()
	/*!
	\param type a const SystemType - The system type
	*/
	void SystemManager::addSystem(const SystemType type)
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
					case SystemType::Logger:
					{
						s_activeSystems.push_back(new Log);
						break;
					}
					case SystemType::Randomiser:
					{
						s_activeSystems.push_back(new Randomiser);
						break;
					}
					case SystemType::TimerSystem:
					{
						s_activeSystems.push_back(new TimerSystem);
						break;
					}
					case SystemType::WindowAPISystem:
					{
#ifdef NG_PLATFORM_WINDOWS
						s_activeSystems.push_back(new GLFWSystem);
#endif
						break;
					}
					case SystemType::WindowManager:
					{
						// Check if window API has been added, cannot add window manager if it hasn't
						if (containsSystem(SystemType::WindowAPISystem))
							s_activeSystems.push_back(new WindowManager);
						else
						{
							ENGINE_ERROR("[SystemManager::addSystem] Cannot add WindowManager system until Windowing API system has been added.");
							return;
						}
						break;
					}
					case SystemType::EventManager:
					{
						s_activeSystems.push_back(new EventManager);
						break;
					}
					case SystemType::ResourceManager:
					{
						s_activeSystems.push_back(new ResourceManager);
						break;
					}
					case SystemType::SceneManager:
					{
						s_activeSystems.push_back(new SceneManager);
						break;
					}
					case SystemType::FontManager:
					{
						s_activeSystems.push_back(new FontManager);
						break;
					}
					case SystemType::RenderSystem:
					{
						s_activeSystems.push_back(new RenderSystem);
						break;
					}
					case SystemType::ThreadManager:
					{
						s_activeSystems.push_back(new ThreadManager);
						break;
					}
					default:
						break;
					}

					// System has been added, start the system
					auto system = getSystemByType(type);
					if(system) system->start();
				}
				else
					ENGINE_ERROR("[SystemManager::addSystem] Cannot add system as system type already exists: {0}", static_cast<int>(type));
			}
			else
				ENGINE_ERROR("[SystemManager::addSystem] Cannot add system has the maximum number of systems have been added. System Type: {0}", static_cast<int>(type));
		}
		else
			ENGINE_ERROR("[SystemManager::addSystem] The system manager has not been initialised.");
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
				// Go through the list in reverse order
				for (std::vector<System*>::reverse_iterator i = s_activeSystems.rbegin(); i != s_activeSystems.rend(); ++i)
				{
					// Stop the system
					// then delete the raw pointer
					if ((*i))
					{
						(*i)->stop();
						if((*i)) delete (*i);
					}
					else
						ENGINE_ERROR("[SystemManager::destroy] System is a null pointer.");
				}

				// All systems have been stopped and then deleted, clear the list
				s_activeSystems.clear();
			}
			else
				ENGINE_ERROR("[SystemManager::destroy] Cannot remove systems as system list is empty.");

			s_enabled = false;
		}
		else
			ENGINE_ERROR("[SystemManager::destroy] The system manager has not been initialised.");

	}

	//! getSystemByType()
	/*!
	\param type a const SystemType - The system type
	\return a System* - The system pointer
	*/
	System* SystemManager::getSystemByType(const SystemType type)
	{
		if (s_enabled)
		{
			// Loop through each item and check the system type
			for (auto& system : s_activeSystems)
			{
				if (system)
				{
					if (system->getSystemType() == type)
						return system;
				}
			}
		}
		else
			ENGINE_ERROR("[SystemManager::getSystemByType] The system manager has not been initialised.");
		return nullptr;
	}
}