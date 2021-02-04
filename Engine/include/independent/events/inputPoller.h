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

namespace Engine {
	/*! \class InputPoller
	* \brief API agnostic Input poller for getting current keyboard/mouse state
	*/
	class InputPoller {
	public:
		static std::vector<uint32_t> isAnyKeyPressed(); //!< Is any key pressed, returns list of all keys currently pressed
		static std::vector<uint32_t> isAnyMouseButtonPressed(); //!< Is any mouse button pressed, returns list of all mouse buttons currently pressed
		static bool isKeyPressed(const int32_t keyCode); //!< Is the key pressed
		static bool isMousePressed(const int32_t mouseButton); //!< Is the mouse button pressed
		static glm::vec2 getMousePosition(); //!< Get the current mouse position
		inline static float getMouseX() { return getMousePosition().x; } //!< Get the mouse x position
			/*!< \return a float - The mouse x position */
		inline static float getMouseY() { return getMousePosition().y; } //!< Get the mouse y position
			/*!< \return a float - The mouse y position */
	};
}
#endif