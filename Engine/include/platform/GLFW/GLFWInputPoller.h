/*! \file GLFWInputPoller.h 
*
* \brief A GLFW input poller
*
* \author DMU Course material
*
*/
#ifndef GLFWINPUTPOLLER_H
#define GLFWINPUTPOLLER_H

#include "independent/core/common.h"

namespace Engine 
{
	/*! \class GLFWInputPoller
	* \brief GFLW specific Input poller for getting current keyboard/mouse state
	*/
	class GLFWInputPoller 
	{
	public:
		static std::vector<uint32_t> isAnyKeyPressed(); //!< Is any key pressed, returns list of all keys currently pressed
		static std::vector<uint32_t> isAnyMouseButtonPressed(); //!< Is any mouse button pressed, returns list of all mouse buttons currently pressed
		static bool isKeyPressed(const int32_t keyCode); //!< Is the key pressed
		static bool isMousePressed(const int32_t mouseButton); //!< Is the mouse button pressed
		static glm::vec2 getMousePosition(); //!< Get the current mouse position
	};
}
#endif