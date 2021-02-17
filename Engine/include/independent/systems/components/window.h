/*! \file window.h 
*
* \brief A windows abstract class. All windows implementations should code to this interface.
*
* \author Daniel Bullin
*
*/
#ifndef WINDOW_H
#define WINDOW_H

#include "independent/core/common.h"
#include "independent/systems/components/windowProperties.h"
#include "independent/core/graphicsContext.h"
#include "independent/events/events.h"
#include "independent/core/destroyable.h"

namespace Engine 
{
	/*! \class Window
	* \brief Base windows class
	*/
	class Window : public Destroyable
	{
	protected:
		std::string m_windowName; //!< The name of the window
		WindowProperties m_properties; //!< Properties of the window
		Shared<GraphicsContext> m_graphicsContext; //!< A pointer to the graphics context
		virtual void setEventCallbacks() = 0; //!< Set up the event callbacks for the window
	public:
		virtual ~Window(); //!< Destructor

		inline const std::string& getName() const { return m_windowName; }
			/*!< \return a const std::string& - The window name */

		inline WindowProperties& getProperties() { return m_properties; } //!< Get the window properties
			/*!< \return a WindowProperties& - The window properties */

		inline const Shared<GraphicsContext>& getGraphicsContext() { return m_graphicsContext; } //!< Get the graphics context
			/*!< \return a const std::shared_ptr<GraphicsContext>& - The pointer to the graphics context */

		virtual void* getNativeWindow() const = 0; //!< Get the native window
			/*!< \return a void* - A pointer to the native window */

		virtual void onUpdate(const float timestep, const float totalTime) = 0; //!< Update
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */

		virtual void close() = 0; //!< Close the window

		virtual void setTitle(const char* title) = 0; //!< Set the title of the window
			/*!< \param title a const char* - The title of the window */

		virtual void setSize(const glm::ivec2& size) = 0; //!< Set the size of the window
			/*!< \param size a const glm::ivec2& - The size of the window */

		virtual void setPosition(const glm::ivec2& newPosition) = 0; //!< Set the position of the window
			/*!< \param newPosition a const glm::ivec2& - The position of the window */
		virtual void setPosition(const uint32_t x, const uint32_t y) = 0; //!< Set the position of the window
			/*!< \param x a const uint32_t - The x position of the window
				 \param y a const uint32_t - The y position of the window */

		virtual void setVSync(const bool VSync) = 0; //!< Set the VSync
			/*!< \param VSync a const bool - Is VSync enabled */

		virtual void setFullscreen(const bool fullsceen) = 0; //!< Set whether the window is fullscreen
			/*!< \param fullscreen a const bool - Should the window be fullscreen */

		virtual void setMinimised(const bool minimised) = 0; //!< Set whether the window is minimised
			/*!< \param minimised a const bool - Is the window minimised */

		virtual void setOpacity(const float opacity) = 0; //!< Set the opacity of the window
			/*!< \param opacity a const float - The opacity of the window */

		virtual void focus() = 0; //!< Set the window to be focused

		virtual void hide() = 0; //!< Hide the window
		virtual void show() = 0; //!< Show the window

		virtual void onWindowClose(WindowCloseEvent& e) = 0; //!< Called when a window closes
			/*!< \param e a WindowCloseEvent& - A window close event */
		virtual void onWindowResize(WindowResizeEvent& e) = 0; //!< Called when a window is resized
			/*!< \param e a WindowResizeEvent& - A window resize event */
		virtual void onWindowFocus(WindowFocusEvent& e) = 0; //!< Called when a window gains focus
			/*!< \param e a WindowFocusEvent& - A window focus event */
		virtual void onWindowLostFocus(WindowLostFocusEvent& e) = 0; //!< Called when a window loses focus
			/*!< \param e a WindowLostFocusEvent& - A window lost focus event */
		virtual void onWindowMoved(WindowMovedEvent& e) = 0; //!< Called when a window is moved
			/*!< \param e a WindowMovedEvent& - A window moved event */

		virtual void setCursorInputMode(const CursorInputMode mode) = 0; //!< Set the cursor input mode
			/*!< \param mode a const CursorInputMode - The input mode */

		virtual void setIcon(const uint32_t width, const uint32_t height, unsigned char* pixels) = 0; //!< Set the icon of the window
			/*!< \param width a const uint32_t - The width of the icon
				 \param height a const uint32_t - The height of the icon
				 \param pixels an unsigned char* - The pixel data */
		virtual void setIcon(const std::string& filePath) = 0; //!< Set the icon of the window
			/*!< \param filePath a const std::string& - The file path of the Icon */

		virtual void printWindowDetails() = 0; //!< Print the details of the window with logger

		static Window* create(const char* windowName, const WindowProperties& properties = WindowProperties()); //!< Create a window
	};
}
#endif