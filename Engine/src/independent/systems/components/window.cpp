/*! \file window.cpp 
*
* \brief A windows abstract class. All windows implementations should code to this interface.
*
* \author Daniel Bullin
*
*/

#include "independent/systems/components/window.h"
#include "independent/systems/components/scene.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWWindowImplementation.h"
#endif

namespace Engine
{
	//! ~Window()
	Window::~Window()
	{
	}

#ifdef NG_PLATFORM_WINDOWS
	//! create()
	/*!
	\param windowName a const char* - The window name
	\param properties a const WindowProperties& - The window properties
	\return a Window* - A pointer to the window
	*/
	Window* Window::create(const char* windowName, const WindowProperties& properties)
	{
		return new GLFWWindowImplementation(windowName, properties);
	}
#endif

}