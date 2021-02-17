/*! \file GLFWInputPoller.cpp
*
* \brief A GLFW input poller
*
* \author DMU Course material
*
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "platform/GLFW/GLFWInputPoller.h"
#include "independent/systems/systems/windowManager.h"

namespace Engine 
{
	//! isAnyKeyPressed()
	/*!
	\return a std::vector<uint32_t> - A list of all keys currently pressed
	*/
	std::vector<uint32_t> GLFWInputPoller::isAnyKeyPressed()
	{
		// Check each key if its currently pressed and add it to the key list if it is
		std::vector<uint32_t> keysPressed;
		for (int i = 32; i < 349; i++)
			if (isKeyPressed(i))
				keysPressed.push_back(i);

		return keysPressed;
	}

	//! isAnyMouseButtonPressed()
	/*!
	\return a std::vector<uint32_t> - A list of all mouse buttons currently pressed
	*/
	std::vector<uint32_t> GLFWInputPoller::isAnyMouseButtonPressed()
	{
		// Check each mouse button if its currently pressed and add it to the mouse list if it is
		std::vector<uint32_t> mouseButtonsPressed;
		for (int i = 0; i < 3; i++)
			if (isMousePressed(i))
				mouseButtonsPressed.push_back(i);
		
		return mouseButtonsPressed;
	}

	//! isKeyPressed()
	/*!
	\param keyCode a const int32_t - The key pressed
	\return a bool - Is the key pressed
	*/
	bool GLFWInputPoller::isKeyPressed(const int32_t keyCode)
	{
		// If window pointer is valid
		if (WindowManager::getFocusedWindow())
		{
			if (WindowManager::getFocusedWindow()->getNativeWindow())
			{
				// Get the key and check its state
				auto answer = glfwGetKey(static_cast<GLFWwindow*>(WindowManager::getFocusedWindow()->getNativeWindow()), keyCode);
				return answer == InputState::PRESS || answer == InputState::REPEAT;
			}
		}
		return false;
	}

	//! isMousePressed()
	/*!
	\param mouseButton a const int32_t - The mouse button pressed
	\return a bool - Is the mouse button pressed
	*/
	bool GLFWInputPoller::isMousePressed(const int32_t mouseButton)
	{
		// If window pointer is valid
		if (WindowManager::getFocusedWindow())
		{
			if (WindowManager::getFocusedWindow()->getNativeWindow())
			{
				// Get the mouse button's state
				auto answer = glfwGetMouseButton(static_cast<GLFWwindow*>(WindowManager::getFocusedWindow()->getNativeWindow()), mouseButton);
				return answer == InputState::PRESS;
			}
		}
		return false;
	}

	//! getMousePosition()
	/*!
	\return a glm::vec2 - The mouse position
	*/
	glm::vec2 GLFWInputPoller::getMousePosition()
	{
		// If the window is valid
		if (WindowManager::getFocusedWindow())
		{
			// Get mouse pos
			double x, y;
			glfwGetCursorPos(static_cast<GLFWwindow*>(WindowManager::getFocusedWindow()->getNativeWindow()), &x, &y);
			return glm::vec2(static_cast<float>(x), static_cast<float>(y));
		}
		return glm::vec2(-1.f, 1.f);
	}
}