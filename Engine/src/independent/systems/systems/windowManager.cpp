/*! \file windowManager.cpp
*
* \brief A window manager system which manages windows
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/eventManager.h"

namespace Engine
{
	std::map<std::string, Window*> WindowManager::s_registeredWindows = std::map<std::string, Window*>(); //!< Create empty list
	Window* WindowManager::s_focusedWindow = nullptr; //!< Set to null
	bool WindowManager::s_enabled = false; //!< Set to false

	//! WindowManager()
	WindowManager::WindowManager() : System(Systems::Type::WindowManager)
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
				delete window.second;

			s_focusedWindow = nullptr;
			s_registeredWindows.clear();
			s_enabled = false;
		}
	}

	//! registerWindow()
	/*!
	\param name a const char* - The name of the window
	\param props a const WindowProperties& - The properties of the window
	*/
	void WindowManager::registerWindow(const char* name, const WindowProperties& props)
	{
		if (s_enabled)
		{
			// Check if name is unique, if it is create a window
			if (s_registeredWindows.find(name) == s_registeredWindows.end())
			{
				// Disable multi-windowing until multi-window rendering is support
				if (s_registeredWindows.size() == 1)
					return;

				s_registeredWindows[name] = Window::create(name, props);
				// Call onFocus for the new window which will be brought to focus
				WindowFocusEvent e;
				EventManager::onWindowFocus(s_registeredWindows[name], e);
			}
			else
				ENGINE_ERROR("[WindowManager::registerWindow] Window name {0} is already taken.", name);
		}
		else
			ENGINE_ERROR("[WindowManager::registerWindow] This system has not been enabled.");
	}

	//! deregisterWindow()
	/*!
	\param name a const char* - The name of the window
	*/
	void WindowManager::deregisterWindow(const char* name)
	{
		if (s_enabled)
		{
			// Check if window exists, close it if it does
			if (s_registeredWindows.find(name) != s_registeredWindows.end())
				s_registeredWindows[name]->close();
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
				if (it->second->getDeletion())
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
		else
			ENGINE_ERROR("[WindowManager::deregisterScheduledWindows] This system has not been enabled.");
	}

	//! getWindowByName()
	/*!
	\param name a const char* - The name of the window
	\return a Window* - A pointer to the window
	*/
	Window* WindowManager::getWindowByName(const char* name)
	{
		if (s_enabled)
		{
			// Check if window exists
			if (s_registeredWindows.find(name) != s_registeredWindows.end())
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
		if (!s_enabled)
			ENGINE_ERROR("[WindowManager::getFocusedWindow] This system has not been enabled.");
		return s_focusedWindow;
	}

	//! getRegisteredWindows()
	/*!
	\return a std::map<std::string, Window*>& - A list of all the registered windows
	*/
	std::map<std::string, Window*>& WindowManager::getRegisteredWindows()
	{
		if (!s_enabled)
			ENGINE_ERROR("[WindowManager::getRegisteredWindows] This system has not been enabled.");
		return s_registeredWindows;
	}

	//! setFocusedWindowByName()
	/*!
	\param windowName a const char* - The name of the window
	*/
	void WindowManager::setFocusedWindowByName(const char* windowName)
	{
		if (s_enabled)
		{
			// Check if window exists
			if (s_registeredWindows.find(windowName) != s_registeredWindows.end())
			{
				// Set the currently focused window
				s_focusedWindow = s_registeredWindows[windowName];
				s_registeredWindows[windowName]->focus();
			}
			else
				ENGINE_ERROR("[WindowManager::setFocusedWindowByName] The window cannot be found. Name: {0}.", windowName);
		}
		else
			ENGINE_ERROR("[WindowManager::setFocusedWindowByName] This system has not been enabled.");
	}

	//! setFullscreenWindowByName()
	/*!
	\param windowName a const char* - The name of the window
	*/
	void WindowManager::setFullscreenWindowByName(const char* windowName)
	{
		if (s_enabled)
		{
			// Check if window exists
			if (s_registeredWindows.find(windowName) != s_registeredWindows.end())
			{
				s_registeredWindows[windowName]->setFullscreen(true);
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
			ENGINE_TRACE("Focused Window Address: {0} [Name: {1}]", (void*)s_focusedWindow, s_focusedWindow->getName());
			ENGINE_TRACE("Registered Window Count: {0}", s_registeredWindows.size());
			ENGINE_TRACE("==========================");

			// Loop through each window and print their details
			for (auto& window : s_registeredWindows)
				window.second->printWindowDetails();
		}
		else
			ENGINE_ERROR("[WindowManager::printWindowManagerDetails] This system has not been enabled.");
	}
}