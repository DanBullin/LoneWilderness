/*! \file windowProperties.h
*
* \brief The properties of a window, all data and functions in here must be API agnostic
*
* \author Daniel Bullin
*
*/
#ifndef WINDOWPROPERTIES_H
#define WINDOWPROPERTIES_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \enum CursorInputMode
	* \brief The type of cursor input for the window
	*/
	enum class CursorInputMode
	{
		Visible = 0, Hidden = 1, Disabled = 2
	};

	//! toCursorInputMode()
	/*!
	\param a const uint32_t - An unsigned integer value representing the input mode
	\return a CursorInputMode - The type of cursor input for the window
	*/
	static CursorInputMode toCursorInputMode(const uint32_t value)
	{
		switch (value)
		{
			case 0:
				return CursorInputMode::Visible;
			case 1:
				return CursorInputMode::Hidden;
			case 2:
				return CursorInputMode::Disabled;
			default:
				return CursorInputMode::Visible;
		}
	}

	/*! \class WindowProperties
	* \brief Contains the properties of the window
	*/
	class WindowProperties
	{
	public:
		std::string m_title;			   //!< Title of the window
		uint32_t m_width;				   //!< The width of the window
		uint32_t m_height;				   //!< The height of the window
		float m_aspectRatio;			   //!< The aspect ratio of the window
		glm::ivec2 m_position;			   //!< The position of the window
		bool m_vSync;					   //!< Is VSync enabled
		bool m_fullScreen;				   //!< Is the window fullscreen
		bool m_minimised;				   //!< Is the window currently minimised
		float m_opacity;				   //!< The opacity of the window
		bool m_focused;					   //!< Is the window currently focused
		bool m_hidden;					   //!< Is the window hidden
		CursorInputMode m_cursorInputMode; //!< The cursor input mode (Visible, hidden, etc)
	public:
		WindowProperties(const std::string& title = "Default", const uint32_t width = 800, const uint32_t height = 600,
			const glm::ivec2& position = glm::ivec2(100, 100), const bool vSync = true, const bool fullScreen = false,
			const bool minimised = false, const float opacity = 1.f, const bool focused = true, const bool hide = false,
			const CursorInputMode mode = CursorInputMode::Visible); //!< Constructor
		~WindowProperties(); //!< Destructor

		const std::string& getTitle() const; //!< Get the title of the window
		void setTitle(const std::string& title); //!< Set the title of the window

		const uint32_t getWidth() const; //!< Get the width of the window
		const float getWidthf() const; //!< Get the width of the window as a float
		const uint32_t getHeight() const; //!< Get the height of the window
		const float getHeightf() const; //!< Get the height of the window as a float
		const glm::vec2 getSizef() const; //!< Get the size of the window as floats
		const glm::ivec2 getSize() const; //!< Get the size of the window as integers
		void setSize(const glm::ivec2& size); //!< Set the size of the window
		void setSize(const uint32_t width, const uint32_t height); //!< Set the size of the window

		const float getAspectRatio() const; //!< Get the aspect ratio
		void setAspectRatiof(const float width, const float height); //!< Set the aspect ratio
		void setAspectRatio(const uint32_t width, const uint32_t height); //!< Set the aspect ratio
		void setAspectRatiof(const glm::vec2& dimensions); //!< Set the aspect ratio
		void setAspectRatio(const glm::ivec2& dimensions); //!< Set the aspect ratio

		const glm::ivec2& getPosition() const; //!< Get the position of the window
		void setPosition(const glm::ivec2& newPosition); //!< Set the position of the window

		const bool getVSync() const; //!< Get if window has vsync enabled
		void setVSync(const bool enable); //!< Set whether window has vsync enabled

		const bool getFullScreen() const; //!< Get if window is fullscreen
		void setFullScreen(const bool fullscreen); //!< Set whether the window is fullscreen

		const bool getMinimised() const; //!< Get if window is minimised
		void setMinimised(const bool minimise); //!< Set whether the window is minimised

		const float getOpacity() const; //!< Get the window's opacity
		void setOpacity(const float opacityValue); //!< Set the window's opacity (0.0 - 1.0)

		const bool getFocused() const; //!< Get if window is focused
		void setFocused(const bool focus); //!< Set whether the window is focused

		const bool getHidden() const; //!< Get if the window is hidden
		void setHidden(const bool hide); //!< Set if the window is hidden

		void setCursorInputMode(const CursorInputMode mode); //!< Set the cursor input mode
		const CursorInputMode getCursorInputMode() const; //!< Get the cursor input mode
	};
}
#endif