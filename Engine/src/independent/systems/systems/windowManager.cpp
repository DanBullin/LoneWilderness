/*! \file windowManager.cpp
*
* \brief A window manager system which manages windows
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/eventManager.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	std::map<std::string, Window*> WindowManager::s_registeredWindows = std::map<std::string, Window*>(); //!< Create empty list
	Window* WindowManager::s_focusedWindow = nullptr; //!< Set to null
	bool WindowManager::s_enabled = false; //!< Set to false

	//! WindowManager()
	WindowManager::WindowManager() : System(SystemType::WindowManager)
	{
	}

	//! ~WindowManager()
	WindowManager::~WindowManager()
	{
	}

	//! start()
	void WindowManager::start()
	{
		// Start system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[WindowManager::start] Starting the window manager.");
			s_enabled = true;

			// Create the main window right here before further systems are loaded
			nlohmann::json configData = ResourceManager::getJSON("assets/config.json")["window"][0];
			WindowProperties props(configData["title"].get<std::string>(),
				configData["size"][0],
				configData["size"][1],
				{ configData["position"][0], configData["position"][1] },
				configData["vSync"].get<bool>(),
				configData["fullScreen"].get<bool>(),
				configData["minimised"].get<bool>(),
				configData["opacity"],
				configData["focused"].get<bool>(),
				configData["hide"].get<bool>(),
				toCursorInputMode(configData["cursorInputMode"].get<uint32_t>()));

			auto window = WindowManager::registerWindow(configData["name"].get<std::string>(), props);
			if (window) window->setIcon(configData["icon"].get<std::string>());
		}
	}

	//! stop()
	void WindowManager::stop()
	{
		// Stop system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[WindowManager::stop] Stopping the window manager.");

			// Delete all windows
			for (auto& window : s_registeredWindows)
			{
				if(window.second)
					delete window.second;
			}

			s_registeredWindows.clear();

			// Set the focused window to nullptr
			s_focusedWindow = nullptr;
			s_enabled = false;
		}
	}

	//! windowExists()
	/*!
	\param windowName a const std::string& - The name of the window
	\return a const bool - Was the window name found in the window list
	*/
	const bool WindowManager::windowExists(const std::string& windowName)
	{
		return s_registeredWindows.find(windowName) != s_registeredWindows.end();
	}

	//! registerWindow()
	/*!
	\param name a const std::string& - The name of the window
	\param props a const WindowProperties& - The properties of the window
	\return a Window* - A pointer to the window
	*/
	Window* WindowManager::registerWindow(const std::string& name, const WindowProperties& props)
	{
		if (s_enabled)
		{
			// Check if name is unique, if it is create a window
			if (!windowExists(name))
			{
				////
				// DISABLED MULTIPLE WINDOWS BECAUSE MULTIPLE GRAPHICS CONTEXTS NOT YET SUPPORTED
				// REMOVE THE BELOW CODE WHEN YOU WANT TO REENABLE MULTIPLE WINDOWS
				////
				if (s_registeredWindows.size() == 1)
				{
					ENGINE_ERROR("[WindowManager::registerWindow] Maximum number of windows has been reached.");
					return nullptr;
				}

				s_registeredWindows[name] = Window::create(name, props);
				if(s_registeredWindows[name]) setFocusedWindowByName(name);
				return s_registeredWindows[name];
			}
			else
				ENGINE_ERROR("[WindowManager::registerWindow] Window name {0} is already taken.", name);
		}
		else
			ENGINE_ERROR("[WindowManager::registerWindow] This system has not been enabled.");

		return nullptr;
	}

	//! deregisterWindow()
	/*!
	\param name a const std::string& - The name of the window
	*/
	void WindowManager::deregisterWindow(const std::string& name)
	{
		if (s_enabled)
		{
			// Check if window exists, close it if it does
			if (windowExists(name))
			{
				if(s_registeredWindows[name])
					s_registeredWindows[name]->close();
			}
			else
				ENGINE_ERROR("[WindowManager::deregisterWindow] The window name cannot be found. Name: {0}.", name);
		}
		else
			ENGINE_ERROR("[WindowManager::deregisterWindow] This system has not been enabled.");
	}

	//! deregisterScheduledWindows()
	void WindowManager::deregisterScheduledWindows()
	{
		if (s_enabled)
		{
			// Loop through all registered windows and check if it is scheduled for deletion
			for (auto it = s_registeredWindows.cbegin(); it != s_registeredWindows.cend();)
			{
				if (it->second)
				{
					if (it->second->getDestroyed())
					{
						// Delete window
						if (s_focusedWindow == it->second)
							s_focusedWindow = nullptr;

						delete it->second;
						s_registeredWindows.erase((it++)->first);
					}
					else
						++it;
				}
			}
		}
		else
			ENGINE_ERROR("[WindowManager::deregisterScheduledWindows] This system has not been enabled.");
	}

	//! getWindowByName()
	/*!
	\param name a const std::string& - The name of the window
	\return a Window* - A pointer to the window
	*/
	Window* WindowManager::getWindowByName(const std::string& name)
	{
		if (s_enabled)
		{
			// Check if window exists
			if (windowExists(name))
				return s_registeredWindows[name];
			else
				ENGINE_ERROR("[WindowManager::getWindowByName] The window cannot be found. Name: {0}.", name);
		}
		else
			ENGINE_ERROR("[WindowManager::getWindowByName] This system has not been enabled.");
		return nullptr;
	}

	//! getFocusedWindow()
	/*!
	\return a Window* - A pointer to the window currently focusued
	*/
	Window* WindowManager::getFocusedWindow()
	{
		return s_focusedWindow;
	}

	//! getRegisteredWindows()
	/*!
	\return a std::map<std::string, Window*>& - A list of all the registered windows
	*/
	std::map<std::string, Window*>& WindowManager::getRegisteredWindows()
	{
		return s_registeredWindows;
	}

	//! setFocusedWindowByName()
	/*!
	\param windowName a const std::string& - The name of the window
	*/
	void WindowManager::setFocusedWindowByName(const std::string& windowName)
	{
		if (s_enabled)
		{
			// Check if window exists
			if (windowExists(windowName))
			{
				if (s_registeredWindows[windowName])
				{
					// Set the currently focused window
					s_focusedWindow = s_registeredWindows[windowName];
					s_registeredWindows[windowName]->focus();
				}
				else
					ENGINE_ERROR("[WindowManager::setFocusedWindowByName] Not a valid window. Name: {0}.", windowName);
			}
			else
				ENGINE_ERROR("[WindowManager::setFocusedWindowByName] The window cannot be found. Name: {0}.", windowName);
		}
		else
			ENGINE_ERROR("[WindowManager::setFocusedWindowByName] This system has not been enabled.");
	}

	//! setFullscreenWindowByName()
	/*!
	\param windowName a const std::string& - The name of the window
	*/
	void WindowManager::setFullscreenWindowByName(const std::string& windowName)
	{
		if (s_enabled)
		{
			// Check if window exists
			if (windowExists(windowName))
			{
				if (s_registeredWindows[windowName])
				{
					s_registeredWindows[windowName]->setFullscreen(true);
				}
				else
					ENGINE_ERROR("[WindowManager::setFullscreenWindowByName] Not a valid window. Name: {0}.", windowName);
			}
			else
				ENGINE_ERROR("[WindowManager::setFullscreenWindowByName] The window cannot be found. Name: {0}.", windowName);
		}
		else
			ENGINE_ERROR("[WindowManager::setFullscreenWindowByName] This system has not been enabled.");
	}

	//! printWindowManagerDetails()
	void WindowManager::printWindowManagerDetails()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("==========================");
			ENGINE_TRACE("Window Manager Details");
			ENGINE_TRACE("Focused Window Address: {0}", (void*)s_focusedWindow);
			if(s_focusedWindow) ENGINE_TRACE("Focused Window Name: {0}", s_focusedWindow->getName());
			ENGINE_TRACE("Registered Window Count: {0}", s_registeredWindows.size());
			ENGINE_TRACE("==========================");

			// Loop through each window and print their details
			for (auto& window : s_registeredWindows)
			{
				if(window.second)
					window.second->printWindowDetails();
			}
		}
		else
			ENGINE_ERROR("[WindowManager::printWindowManagerDetails] This system has not been enabled.");
	}
}