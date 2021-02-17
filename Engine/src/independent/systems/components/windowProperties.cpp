/*! \file windowProperties.cpp
*
* \brief The properties of a window, all data and functions in here must be API agnostic
*
* \author Daniel Bullin
*
*/
#include "independent/systems/components/windowProperties.h"

namespace Engine
{
	//! WindowProperties()
	/*!
	\param title a const char* - The window title
	\param width a const uint32_t - The window width
	\param height a const uint32_t - The window height
	\param position a const glm::ivec2& - The window position
	\param vSync a const bool - Is VSync enabled
	\param fullScreen a const bool - Is the window fullscreen
	\param minimised a const bool - Is the window minimised
	\param opacity a const float - The window opacity
	\param focused a const bool - Is the window focused
	\param hide a const bool - Is the window hidden
	\param mode a const CursorInputMode - The cursor input mode
	*/
	WindowProperties::WindowProperties(const char* title, const uint32_t width, const uint32_t height, const glm::ivec2& position, const bool vSync, const bool fullScreen, const bool minimised, const float opacity, const bool focused, const bool hide, const CursorInputMode mode)
		: m_title(title), m_width(width), m_height(height), m_aspectRatio(static_cast<float>(width) / static_cast<float>(height)), m_position(position),
		m_vSync(vSync), m_fullScreen(fullScreen), m_minimised(minimised), m_opacity(opacity), m_focused(focused), m_hidden(hide), m_cursorInputMode(mode)
	{
	}

	//! ~WindowProperties()
	WindowProperties::~WindowProperties()
	{
	}

	//! getTitle()
	/*!
	\return a const std::string& - The window's title
	*/
	const std::string& WindowProperties::getTitle() const
	{
		return m_title;
	}

	//! setTitle()
	/*!
	\param title a const char* - The window's title
	*/
	void WindowProperties::setTitle(const char* title)
	{
		m_title = title;
	}

	//! getWidth()
	/*!
	\return a const uint32_t - The window's width
	*/
	const uint32_t WindowProperties::getWidth() const
	{
		return m_width;
	}

	//! getWidthf()
	/*!
	\return a const float - The window's width
	*/
	const float WindowProperties::getWidthf() const
	{
		return static_cast<float>(m_width);
	}

	//! getHeight()
	/*!
	\return a const uint32_t - The window's height
	*/
	const uint32_t WindowProperties::getHeight() const
	{
		return m_height;
	}

	//! getHeightf()
	/*!
	\return a const float - The window's height
	*/
	const float WindowProperties::getHeightf() const
	{
		return static_cast<float>(m_height);
	}

	//! getSizef()
	/*!
	\return a const glm::vec2 - The window size
	*/
	const glm::vec2 WindowProperties::getSizef() const
	{
		return glm::vec2(static_cast<float>(m_width), static_cast<float>(m_height));
	}

	//! getSize()
	/*!
	\return a const glm::ivec2 - The window size
	*/
	const glm::ivec2 WindowProperties::getSize() const
	{
		return glm::ivec2(m_width, m_height);
	}
	
	//! setSize()
	/*!
	\param size a const glm::ivec2& - The window size
	*/
	void WindowProperties::setSize(const glm::ivec2& size)
	{
		m_width = size.x;
		m_height = size.y;
	}

	//! setSize()
	/*!
	\param width a const uint32_t - The window width
	\param height a const uint32_t - The window height
	*/
	void WindowProperties::setSize(const uint32_t width, const uint32_t height)
	{
		m_width = width;
		m_height = height;
	}

	//! getAspectRatio()
	/*!
	\return a const float - The window's aspect ratio
	*/
	const float WindowProperties::getAspectRatio() const
	{
		return m_aspectRatio;
	}

	//! setAspectRatiof()
	/*!
	\param width a const float - The width
	\param height a const float - The height
	*/
	void WindowProperties::setAspectRatiof(const float width, const float height)
	{
		m_aspectRatio = width / height;
	}

	//! setAspectRatio()
	/*!
	\param width a const uint32_t - The width
	\param height a const uint32_t - The height
	*/
	void WindowProperties::setAspectRatio(const uint32_t width, const uint32_t height)
	{
		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	//! setAspectRatiof()
	/*!
	\param dimensions a const glm::vec2& - The size
	*/
	void WindowProperties::setAspectRatiof(const glm::vec2& dimensions)
	{
		m_aspectRatio = dimensions.x / dimensions.y;
	}

	//! setAspectRatio()
	/*!
	\param dimensions a const glm::ivec2& - The size
	*/
	void WindowProperties::setAspectRatio(const glm::ivec2& dimensions)
	{
		m_aspectRatio = static_cast<float>(dimensions.x) / static_cast<float>(dimensions.y);
	}

	//! getPosition()
	/*!
	\return a const glm::ivec2& - The position of the window
	*/
	const glm::ivec2& WindowProperties::getPosition() const
	{
		return m_position;
	}

	//! setPosition()
	/*!
	\param newPosition a const glm::ivec2& - The new position of the window
	*/
	void WindowProperties::setPosition(const glm::ivec2& newPosition)
	{
		m_position = newPosition;
	}

	//! getVSync()
	/*!
	\return a const bool - Is VSync enabled
	*/
	const bool WindowProperties::getVSync() const
	{
		return m_vSync;
	}

	//! setVSync()
	/*!
	\param enable a const bool - Enable/Disable VSync
	*/
	void WindowProperties::setVSync(const bool enable)
	{
		m_vSync = enable;
	}

	//! getFullScreen()
	/*!
	\return a const bool - Is the window fullscreen
	*/
	const bool WindowProperties::getFullScreen() const
	{
		return m_fullScreen;
	}

	//! setFullScreen()
	/*!
	\param fullscreen a const bool - Set whether the window is fullscreen
	*/
	void WindowProperties::setFullScreen(const bool fullscreen)
	{
		m_fullScreen = fullscreen;
	}

	//! getMinimised()
	/*!
	\return a const bool - Is the window minimised
	*/
	const bool WindowProperties::getMinimised() const
	{
		return m_minimised;
	}

	//! setMinimised()
	/*!
	\param minimise a const bool - Is the window minimised
	*/
	void WindowProperties::setMinimised(const bool minimise)
	{
		m_minimised = minimise;
	}

	//! getOpacity()
	/*!
	\return a const float - The window's opacity value
	*/
	const float WindowProperties::getOpacity() const
	{
		return m_opacity;
	}

	//! setOpacity()
	/*!
	\param opacityValue a const float - The opacity value
	*/
	void WindowProperties::setOpacity(const float opacityValue)
	{
		m_opacity = opacityValue;
	}

	//! getFocused()
	/*!
	\return a const bool - Is the window focused
	*/
	const bool WindowProperties::getFocused() const
	{
		return m_focused;
	}

	//! setFocused()
	/*!
	\param focus a const bool - Is the window focused
	*/
	void WindowProperties::setFocused(const bool focus)
	{
		m_focused = focus;
	}

	//! getHidden()
	/*!
	\return a const bool - Is the window hidden
	*/
	const bool WindowProperties::getHidden() const
	{
		return m_hidden;
	}

	//! setHidden()
	/*!
	\param hide a const bool - Is the window hidden
	*/
	void WindowProperties::setHidden(const bool hide)
	{
		m_hidden = hide;
	}

	//! setCursorInputMode()
	/*!
	\param mode a const CursorInputMode - The cursur input mode
	*/
	void WindowProperties::setCursorInputMode(const CursorInputMode mode)
	{
		m_cursorInputMode = mode;
	}

	//! getCursorInputMode()
	/*!
	\return a const CursorInputMode - The cursur input mode
	*/
	const CursorInputMode WindowProperties::getCursorInputMode() const
	{
		return m_cursorInputMode;
	}
}