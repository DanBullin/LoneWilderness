/*! \file GLFWSystem.cpp
*
* \brief A GLFW System windowing API system
*
* \author DMU Course material
*
*/

#include "platform/GLFW/systems/GLFWSystem.h"
#include "independent/systems/systems/log.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	bool GLFWSystem::s_enabled = false; //!< Set to false

	//! GLFWSystem()
	GLFWSystem::GLFWSystem() : System(Systems::Type::WindowAPISystem)
	{
	}

	//! ~GLFWSystem()
	GLFWSystem::~GLFWSystem()
	{
	}

	//! start()
	void GLFWSystem::start()
	{
		// Start system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[GLFWSystem::start] Starting the GLFW system.");
			auto errorCode = glfwInit();

			if (!errorCode)
				ENGINE_ERROR("[GLFWSystem::start] Cannot initialise GLFW: {0}.", errorCode);

			s_enabled = true;
		}
	}

	//! stop()
	void GLFWSystem::stop()
	{
		// Stop system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[GLFWSystem::stop] Stopping the GLFW system.");
			// Terminate GLFW
			glfwTerminate();
			s_enabled = false;
		}
	}
}