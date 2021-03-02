/*! \file GLFWWinImplementation.cpp
*
* \brief A GLFW Window implementation
*
* \author Daniel Bullin
*
*/
#include "platform/GLFW/GLFWWindowImplementation.h"
#include "platform/GLFW/GLFWGLGraphicsContext.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/eventManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine
{
	//! GLFWWindowImplementation()
	/*!
	\param windowName a const std::string& - The name of the window
	\param properties a const WindowProperties& - The window properties
	*/
	GLFWWindowImplementation::GLFWWindowImplementation(const std::string& windowName, const WindowProperties& properties)
	{
		// Initiate the window
		m_windowName = windowName;
		m_properties = properties;
		m_properties.setAspectRatio(m_properties.getSizef());

		m_monitor = glfwGetPrimaryMonitor();

		if (!m_monitor)
			ENGINE_ERROR("[GLFWWindowImplementation::GLFWWindowImplementation] Couldn't get primary monitor for window: {0}.", windowName);

		// Create the GLFW object
		// Check fullscreen is enabled, give primary monitor if so, otherwise null
		m_native = glfwCreateWindow(m_properties.getWidth(), m_properties.getHeight(), m_properties.getTitle().c_str(), nullptr, nullptr);

		if (!m_native)
			ENGINE_ERROR("[GLFWWindowImplementation::GLFWWindowImplementation] Couldn't create GLFW window: {0}.", windowName);

		m_graphicsContext.reset(new GLFWGLGraphicsContext(m_native));
		m_graphicsContext->init();

		// Set this window as user data in window
		if(m_native) glfwSetWindowUserPointer(m_native, static_cast<void*>(this));

		// Set window properties
		setPosition(m_properties.getPosition());
		setMinimised(m_properties.getMinimised());
		setOpacity(m_properties.getOpacity());
		setCursorInputMode(m_properties.getCursorInputMode());
		setVSync(m_properties.getVSync());
		
		// If the window is hidden, hide it otherwise show
		if (properties.getHidden())
			hide();
		else
			show();

		// Set the window event callbacks
		setEventCallbacks();

		// If the window is to be fullscreen, tmp set it to false and set the window to fullscreen
		if (m_properties.getFullScreen())
		{
			m_properties.setFullScreen(false);
			setFullscreen(true);
		}

		// Print the window's details upon creation
		ENGINE_TRACE("[GLFWWindowImplementation::GLFWWindowImplementation] Window details upon creation for: {0}.", m_windowName);
		printWindowDetails();
	}

	//! ~GLFWWindowImplementation()
	GLFWWindowImplementation::~GLFWWindowImplementation()
	{
		// Destroy the window
		ENGINE_INFO("[GLFWWindowImplementation::~GLFWWindowImplementation] Closing the window.");
		if(m_native) glfwDestroyWindow(m_native);
		m_native = nullptr;
	}

	//! getNativeWindow()
	/*!
	\return a void* - A pointer to the native GLFW window
	*/
	void* GLFWWindowImplementation::getNativeWindow() const
	{
		return m_native;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void GLFWWindowImplementation::onUpdate(const float timestep, const float totalTime)
	{
		if (m_native)
		{
			m_graphicsContext->swapBuffers();
			glfwPollEvents();
		}
	}

	//! onWindowClose()
	/*!
	\param e a WindowCloseEvent& - Reference to the window close event
	*/
	void GLFWWindowImplementation::onWindowClose(WindowCloseEvent& e)
	{
		close();
	}

	//! onWindowResize()
	/*!
	\param e a WindowResizeEvent& - Reference to the window resize event
	*/
	void GLFWWindowImplementation::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getSize() != glm::ivec2(0.f, 0.f))
		{
			m_properties.setSize(e.getSize());
			m_properties.setAspectRatio(e.getSize());
			if(m_properties.getMinimised())
				m_properties.setMinimised(false);

			m_graphicsContext->updateViewport(0, 0, e.getWidth(), e.getHeight());
		}
		else
			m_properties.setMinimised(true);
	}

	//! onWindowFocus()
	/*!
	\param e a WindowFocusEvent& - Reference to the window focus event
	*/
	void GLFWWindowImplementation::onWindowFocus(WindowFocusEvent& e)
	{
		m_properties.setFocused(true);
	}

	//! onWindowLostFocus()
	/*!
	\param e a WindowLostFocusEvent& - Reference to the window lost focus event
	*/
	void GLFWWindowImplementation::onWindowLostFocus(WindowLostFocusEvent& e)
	{
		m_properties.setFocused(false);
	}

	//! onWindowMoved()
	/*!
	\param e a WindowMovedEvent& - Reference to the window moved event
	*/
	void GLFWWindowImplementation::onWindowMoved(WindowMovedEvent& e)
	{
		m_properties.setPosition(e.getPos());
	}

	//! close()
	void GLFWWindowImplementation::close()
	{
		if (m_native && !getDestroyed())
		{
			destroy();
		}
	}

	//! setEventCallbacks()
	void GLFWWindowImplementation::setEventCallbacks()
	{
#ifndef ENGINETESTS
		// Set all GLFW callback functions needed

		glfwSetWindowCloseCallback(m_native,
			[](GLFWwindow* window)
		{
			WindowCloseEvent e;
			EventManager::onWindowClose(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
		}
		);

		glfwSetWindowSizeCallback(m_native,
			[](GLFWwindow* window, int newWidth, int newHeight)
		{
			WindowResizeEvent e(newWidth, newHeight);
			EventManager::onWindowResize(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
		}
		);

		glfwSetWindowFocusCallback(m_native,
			[](GLFWwindow* window, int focused)
		{
			if (focused)
			{
				WindowFocusEvent e;
				EventManager::onWindowFocus(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
			}
			else
			{
				WindowLostFocusEvent e;
				EventManager::onWindowLostFocus(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
			}
		}
		);

		glfwSetWindowPosCallback(m_native,
			[](GLFWwindow* window, int xpos, int ypos)
		{
			WindowMovedEvent e(xpos, ypos);
			EventManager::onWindowMoved(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
		}
		);

		glfwSetKeyCallback(m_native,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_RELEASE)
			{
				// Key is released
				KeyReleasedEvent e(key);
				EventManager::onKeyReleased(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
			}
		}
		);

		glfwSetCharCallback(m_native,
			[](GLFWwindow* window, unsigned int codepoint)
		{
			KeyTypedEvent e(codepoint);
			EventManager::onKeyTyped(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
		}
		);

		glfwSetMouseButtonCallback(m_native,
			[](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_RELEASE)
			{
				MouseReleasedEvent e(button);
				EventManager::onMouseReleased(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
			}
		}
		);

		glfwSetCursorPosCallback(m_native,
			[](GLFWwindow* window, double xpos, double ypos)
		{
			MouseMovedEvent e(static_cast<float>(xpos), static_cast<float>(ypos));
			EventManager::onMouseMoved(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
		}
		);

		glfwSetScrollCallback(m_native,
			[](GLFWwindow* window, double xoffset, double yoffset)
		{
			MouseScrolledEvent e(static_cast<float>(xoffset), static_cast<float>(yoffset));
			EventManager::onMouseScrolled(static_cast<GLFWWindowImplementation*>(glfwGetWindowUserPointer(window)), e);
		}
		);

#endif
	}

	//! setTitle()
	/*!
	\param title a const char* - The new title
	*/
	void GLFWWindowImplementation::setTitle(const char* title)
	{
		if (m_native)
		{
			m_properties.setTitle(title);
			glfwSetWindowTitle(m_native, title);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setTitle] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setSize()
	/*!
	\param size a const glm::ivec2& - The new size
	*/
	void GLFWWindowImplementation::setSize(const glm::ivec2& size)
	{
		if (m_native)
		{
			// Size property will be updated automatically
			glfwSetWindowSize(m_native, size.x, size.y);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setSize] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setPosition()
	/*!
	\param newPosition a const glm::ivec2& - The new position
	*/
	void GLFWWindowImplementation::setPosition(const glm::ivec2& newPosition)
	{
		if (m_native)
		{
			// Position property will be updated automatically
			glfwSetWindowPos(m_native, newPosition.x, newPosition.y);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setPosition] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setPosition()
	/*!
	\param x a const uint32_t - The x position of the window
	\param y a const uint32_t - The y position of the window
	*/
	void GLFWWindowImplementation::setPosition(const uint32_t x, const uint32_t y)
	{
		if(m_native)
			setPosition(glm::ivec2(x, y));
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setPosition] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setVSync()
	/*!
	\param VSync a const bool - Enable/Disable VSync
	*/
	void GLFWWindowImplementation::setVSync(const bool VSync)
	{
		if (m_native)
		{
			m_properties.setVSync(VSync);
			glfwSwapInterval(VSync);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setVSync] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setFullscreen()
	/*!
	\param fullscreen a const bool - Will this window be fullscreen
	*/
	void GLFWWindowImplementation::setFullscreen(const bool fullscreen)
	{
		if (fullscreen == m_properties.getFullScreen()) return;

		if (!m_native)
		{
			ENGINE_ERROR("[GLFWWindowImplementation::setFullscreen] Couldn't find GLFW window: {0}.", m_windowName);
			return;
		}

		if (fullscreen)
		{
			// Get resolution of monitor
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			if (!mode)
			{
				ENGINE_ERROR("[GLFWWindowImplementation::setFullscreen] Couldn't find GLFW video mode: {0}.", m_windowName);
				return;
			}

			m_nonFullscreenWindowSize = m_properties.getSize();
			m_nonFullscreenWindowPosition = m_properties.getPosition();

			int width = mode->width;
			int height = mode->height;
			int refresh = mode->refreshRate;

			// Set video to fullscreen
			glfwSetWindowMonitor(m_native, m_monitor, 0, 0, width, height, refresh);
			m_properties.setFullScreen(true);
		}
		else
		{
			// Set video to previous dimensions
			glfwSetWindowMonitor(m_native, nullptr, m_nonFullscreenWindowPosition.x, m_nonFullscreenWindowPosition.y, m_nonFullscreenWindowSize.x, m_nonFullscreenWindowSize.y, 0);
			m_properties.setFullScreen(false);
		}

		glfwFocusWindow(m_native);
	}

	//! setMinimised()
	/*!
	\param minimised a const bool - Minimise the window
	*/
	void GLFWWindowImplementation::setMinimised(const bool minimised)
	{
		if (m_native)
		{
			if (minimised)
				glfwIconifyWindow(m_native);
			else
				glfwRestoreWindow(m_native);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setMinimised] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setOpacity()
	/*!
	\param opacity a const float - The new opacity (0.0 - 1.0)
	*/
	void GLFWWindowImplementation::setOpacity(const float opacity)
	{
		// Opacity must be between 0.0 and 1.0
		if (m_native)
		{
			if (opacity >= 0.f && opacity <= 1.f)
			{
				m_properties.setOpacity(opacity);
				glfwSetWindowOpacity(m_native, opacity);
			}
			else
				ENGINE_ERROR("[GLFWWindowImplementation::setOpacity] Opacity value must be between 0.0 and 1.0.");
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setOpacity] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! focus()
	void GLFWWindowImplementation::focus()
	{
		if (m_native)
		{
			glfwFocusWindow(m_native);
			WindowFocusEvent e;
			onWindowFocus(e);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::focus] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! hide()
	void GLFWWindowImplementation::hide()
	{
		if (m_native)
		{
			m_properties.setHidden(true);
			glfwHideWindow(m_native);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::hide] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! show()
	void GLFWWindowImplementation::show()
	{
		if (m_native)
		{
			m_properties.setHidden(false);
			glfwShowWindow(m_native);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::show] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setCursorInputMode()
	/*!
	\param mode a const CursorInputMode - The cursor input mode
	*/
	void GLFWWindowImplementation::setCursorInputMode(const CursorInputMode mode)
	{
		if (m_native)
		{
			int glfwMode = 0;

			// Set the input mode based on argument
			switch (mode)
			{
				case CursorInputMode::Visible:
				{
					glfwMode = GLFW_CURSOR_NORMAL;
					break;
				}
				case CursorInputMode::Hidden:
				{
					glfwMode = GLFW_CURSOR_HIDDEN;
					break;
				}
				case CursorInputMode::Disabled:
				{
					glfwMode = GLFW_CURSOR_DISABLED;
					break;
				}
			}

			// Tell GLFW the input mode
			m_properties.setCursorInputMode(mode);
			glfwSetInputMode(m_native, GLFW_CURSOR, glfwMode);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setCursorInputMode] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setCursorInputMode()
	/*! 
	\param width a const uint32_t - The width of the icon
	\param height a const uint32_t - The height of the icon
	\param pixels an unsigned char* - The pixel data (4 channel pixel data)
	*/
	void GLFWWindowImplementation::setIcon(const uint32_t width, const uint32_t height, unsigned char* pixels)
	{
		if (m_native)
		{
			GLFWimage icon;
			icon.width = width;
			icon.height = height;
			icon.pixels = pixels;
			glfwSetWindowIcon(m_native, 1, &icon);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setIcon] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! setCursorInputMode()
	/*!
	\param filePath a const std::string& - The filepath of the icon
	*/
	void GLFWWindowImplementation::setIcon(const std::string& filePath)
	{
		if (m_native)
		{
			int width, height, channels;
			unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

			if (channels == 4)
				setIcon(width, height, data);
			else
				ENGINE_ERROR("[GLFWWindowImplementation::setIcon] The icon being loaded is not a 4 channel texture. Texture filepath: {0}.", filePath);

			stbi_image_free(data);
		}
		else
			ENGINE_ERROR("[GLFWWindowImplementation::setIcon] Couldn't find GLFW window: {0}.", m_windowName);
	}

	//! printWindowDetails()
	void GLFWWindowImplementation::printWindowDetails()
	{
		ENGINE_TRACE("========================================================");
		ENGINE_TRACE("Window Properties for {0}:", m_windowName);
		ENGINE_TRACE("Address: {0}", (void*)this);
		ENGINE_TRACE("Title: {0}", m_properties.getTitle());
		ENGINE_TRACE("Size: {0} by {1}", m_properties.getWidth(), m_properties.getHeight());
		ENGINE_TRACE("Aspect Ratio: {0}", m_properties.getAspectRatio());
		ENGINE_TRACE("Position: {0}, {1}", m_properties.getPosition().x, m_properties.getPosition().y);
		ENGINE_TRACE("VSync: {0}", m_properties.getVSync());
		ENGINE_TRACE("Fullscreen: {0}", m_properties.getFullScreen());
		ENGINE_TRACE("Minimised: {0}", m_properties.getMinimised());
		ENGINE_TRACE("Opacity: {0}", m_properties.getOpacity());
		ENGINE_TRACE("Focused: {0}", m_properties.getFocused());
		ENGINE_TRACE("Hidden: {0}", m_properties.getHidden());
		ENGINE_TRACE("CursorInputMode: {0}", static_cast<int>(m_properties.getCursorInputMode()));
		ENGINE_TRACE("Scheduled for Deletion: {0}", getDestroyed());
		ENGINE_TRACE("Address of Graphics Context: {0}", (void*)m_graphicsContext.get());
		ENGINE_TRACE("Non-fullscreen window position: {0}, {1}", m_nonFullscreenWindowPosition.x, m_nonFullscreenWindowPosition.y);
		ENGINE_TRACE("Non-fullscreen window size: {0}, {1}", m_nonFullscreenWindowSize.x, m_nonFullscreenWindowSize.y);
		ENGINE_TRACE("========================================================");
	}
}