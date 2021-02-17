/*! \file GLFWSystem.h
*
* \brief A GLFW System windowing API system
*
* \author DMU Course material
*
*/
#ifndef GLFWSYSTEM_H
#define GLFWSYSTEM_H

#include "independent/systems/system.h"

namespace Engine 
{
	/*! \class GLFWSystem
	* \brief A GLFWSystem handling the GLFW windows
	*/
	class GLFWSystem : public System 
	{
	private:
		static bool s_enabled; //!< Is this GLFW system enabled
	public:
		GLFWSystem(); //!< Constructor
		~GLFWSystem(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system
	};
}
#endif