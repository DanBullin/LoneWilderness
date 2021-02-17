/*! \file inputPoller.h 
*
* \brief An input poller class
*
* \author DMU Course material
*
*/
#ifndef INPUTPOLLER_H
#define INPUTPOLLER_H

#include "independent/core/common.h"

namespace Engine 
{
	/*! \class InputPoller
	* \brief API agnostic Input poller for getting current keyboard/mouse state
	*/
	class InputPoller 
	{
	public:
		static std::vector<uint32_t> isAnyKeyPressed(); //!< Is any key pressed, returns list of all keys currently pressed
		static std::vector<uint32_t> isAnyMouseButtonPressed(); //!< Is any mouse button pressed, returns list of all mouse buttons currently pressed
		static bool isKeyPressed(const int32_t keyCode); //!< Is the key pressed
		static bool isMousePressed(const int32_t mouseButton); //!< Is the mouse button pressed
		static glm::vec2 getMousePosition(); //!< Get the current mouse position
		static float getMouseX(); //!< Get the mouse x position
		static float getMouseY(); //!< Get the mouse y position
	};
}
#endif