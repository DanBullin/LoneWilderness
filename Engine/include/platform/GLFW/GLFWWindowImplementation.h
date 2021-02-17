/*! \file GLFWWindowImplementation.h 
*
* \brief A GLFW Window implementation
*
* \author Daniel Bullin
*
*/
#ifndef GLFWWINDOWIMPLEMENTATION_H
#define GLFWWINDOWIMPLEMENTATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "independent/systems/components/window.h"

namespace Engine 
{
	/*! \class GLFWWindowImplementation
	* \brief Implementation of a window using GLFW
	*/
	class GLFWWindowImplementation : public Window
	{
	private:
		GLFWwindow* m_native; //!< Native GLFW window
		GLFWmonitor* m_monitor; //!< The monitor this window is currently displayed in
		glm::ivec2 m_nonFullscreenWindowPosition; //!< The position of the window before fullscreening
		glm::ivec2 m_nonFullscreenWindowSize; //!< The size of the window before fullscreening
	protected:
		void setEventCallbacks() override; //!< Set up the event callbacks for the window
	public:
		GLFWWindowImplementation(const char* windowName, const WindowProperties& properties); //!< Constructor
		~GLFWWindowImplementation(); //!< Destructor
		void* getNativeWindow() const override; //!< Get the native window
		void onUpdate(const float timestep, const float totalTime) override; //!< Update
		void onWindowClose(WindowCloseEvent& e) override; //!< Called when a window closes
		void onWindowResize(WindowResizeEvent& e) override; //!< Called when a window is resized
		void onWindowFocus(WindowFocusEvent& e) override; //!< Called when a window gains focus
		void onWindowLostFocus(WindowLostFocusEvent& e) override; //!< Called when a window loses focus
		void onWindowMoved(WindowMovedEvent& e) override; //!< Called when a window is moved
		void close() override; //!< Close the window
		void setTitle(const char* title) override; //!< Set the title of the window
		void setSize(const glm::ivec2& size) override; //!< Set the size of the window
		void setPosition(const glm::ivec2& newPosition) override; //!< Set the position of the window
		void setPosition(const uint32_t x, const uint32_t y) override; //!< Set the position of the window
		void setVSync(const bool VSync) override; //!< Set the VSync
		void setFullscreen(const bool fullscreen) override; //!< Set whether the window is fullscreen
		void setMinimised(const bool minimised) override; //!< Set whether the window is minimised
		void setOpacity(const float opacity) override; //!< Set the opacity of the window
		void focus() override; //!< Focus the window
		void hide() override; //!< Hide the window
		void show() override; //!< Show the window
		void setCursorInputMode(const CursorInputMode mode) override; //!< Set the cursor input mode
		void setIcon(const uint32_t width, const uint32_t height, unsigned char* pixels) override; //!< Set the icon of the window
		void setIcon(const std::string& filePath) override; //!< Set the icon of the window by filepath
		void printWindowDetails() override; //!< Print window details with the logger
	};
}
#endif