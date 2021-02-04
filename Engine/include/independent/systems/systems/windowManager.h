/*! \file windowManager.h
*
* \brief A window manager system which manages windows
*
* \author Daniel Bullin
*
*/
#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "independent/core/common.h"
#include "independent/systems/system.h"
#include "independent/systems/components/window.h"

namespace Engine {
	/*! \class WindowManager
	* \brief A window manager which manages windows
	*/
	class WindowManager : public System {
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::map<std::string, Window*> s_registeredWindows; //!< A list of registered windows
		static Window* s_focusedWindow; //!< The window that is currently focused
	public:
		WindowManager(); //!< Constructor
		~WindowManager(); //!< Destructor
		virtual void start() override; //!< Start the system
		virtual void stop() override; //!< Stop the system

		static void registerWindow(const char* name, const WindowProperties& props); //!< Register a window with the window manager
		static void deregisterWindow(const char* name); //!< Deregister a window with the window manager
		static void deregisterScheduledWindows(); //!< Deregister all windows scheduled for deletion

		static Window* getWindowByName(const char* name); //!< Get a window by name
		static Window* getFocusedWindow(); //!< Get the window which is currently focused
		static std::map<std::string, Window*>& getRegisteredWindows(); //!< Get a list of all registered windows

		static void setFocusedWindowByName(const char* windowName); //!< Set a window to be focused
		static void setFullscreenWindowByName(const char* windowName); //!< Set a window to be fullscreen

		static void printWindowManagerDetails(); //!< Print manager details
	};
}
#endif