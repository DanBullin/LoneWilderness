/*! \file windowEvents.h
*
* \brief The types of window events
*
* \author DMU Course material
*
*/
#ifndef WINDOWEVENTS_H
#define WINDOWEVENTS_H

#include "independent/events/event.h"
#include <glm/glm.hpp>

namespace Engine {
	/*! \class WindowCloseEvent
	* \brief A window close event
	*/
	class WindowCloseEvent : public Event 
	{
	public:
		static inline const EventType getStaticType() { return EventType::WindowClose; } //!< Returns the static type of the event
			/*!< \return a const EventType - The static window close event type */
		virtual inline const EventType getEventType() const override { return EventType::WindowClose; } //!< Gets the event type
			/*!< \return a const EventType - The window close event type */
		virtual inline const int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Gets the event category flags
			/*!< \return a const int32_t - The window close's event category flags */
	};

	/*! \class WindowResizeEvent
	* \brief A window resize event
	*/
	class WindowResizeEvent : public Event {
	private:
		int32_t m_width;	//!< Window width after resize
		int32_t m_height;	//!< Window height after resize
	public:
		WindowResizeEvent(const int32_t width, const int32_t height) : m_width(width), m_height(height) {} //!< Constructor
			/*!< \param width a const int32_t - The width of the window resize
				 \param height a const int32_t - The height of the window resize */
		static inline const EventType getStaticType() { return EventType::WindowResize; } //!< Return static type
			/*!< \return a const EventType - The static window close event type */
		virtual inline const EventType getEventType() const override { return EventType::WindowResize; } //!< Gets the event type
			/*!< \return a const EventType - The window close event type */
		virtual inline const int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Gets the event category flags
			/*!< \return a const int32_t - The window close's event category flags */
		inline const int32_t getWidth() const { return m_width; } //!< Return the width of the resize
			/*!< \return a const int32_t - The width of the window */
		inline const int32_t getHeight() const { return m_height; } //!< Return the height of the resize
			/*!< \return a const int32_t - The height of the window */
		inline const glm::ivec2 getSize() const { return { m_width, m_height }; } //!< Returns the size of the window resize
			/*!< \return a const glm::ivec2 - The size of the window */
	};

	/*! \class WindowFocusEvent
	* \brief A window focus event
	*/
	class WindowFocusEvent : public Event {
	public:
		static inline const EventType getStaticType() { return EventType::WindowFocus; } //!< Return static type
			/*!< \return a const EventType - The static window focus event type */
		virtual inline const EventType getEventType() const override { return EventType::WindowFocus; } //!< Gets the event type
			/*!< \return a const EventType - The window focus event type */
		virtual inline const int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Gets the event category flags
			/*!< \return a const int32_t - The window focus's event category flags */
	};

	/*! \class WindowLostFocusEvent
	* \brief A window lost focus event
	*/
	class WindowLostFocusEvent : public Event {
	public:
		static inline const EventType getStaticType() { return EventType::WindowLostFocus; } //!< Return static type
			/*!< \return a const EventType - The static window lost focus event type */
		virtual inline const EventType getEventType() const override { return EventType::WindowLostFocus; } //!< Gets the event type
			/*!< \return a const EventType - The window lost focus event type */
		virtual inline const int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Gets the event category flags
			/*!< \return a const int32_t - The window lost focus's event category flags */
	};

	/*! \class WindowMovedEvent
	* \brief A window moved event
	*/
	class WindowMovedEvent : public Event {
	private:
		int32_t m_xPos; //!< Position of the x-axis of the window after its been moved
		int32_t m_yPos; //!< Position of the y-axis of the window after its been moved
	public:
		WindowMovedEvent(const int32_t x, const int32_t y) : m_xPos(x), m_yPos(y) {} //!< Constructor
			/*!< \param x a const int32_t - The x position of the window
				 \param y a const int32_t - The y position of the window */
		static inline const EventType getStaticType() { return EventType::WindowMoved; } //!< Return static type
			/*!< \return a const EventType - The static window moved event type */
		virtual inline const EventType getEventType() const override { return EventType::WindowMoved; } //!< Gets the event type
			/*!< \return a const EventType - The window moved event type */
		virtual inline const int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Gets the event category flags
			/*!< \return a const int32_t - The window moved's event category flags */
		inline const int32_t getXPos() const { return m_xPos; } //!< Return the x coordinate of the window position
			/*!< \return a const int32_t - The x position of the window */
		inline const int32_t getYPos() const { return m_yPos; } //!< Return the y coordinate of the window position
			/*!< \return a const int32_t - The y position of the window */
		inline const glm::ivec2 getPos() const { return { m_xPos, m_yPos }; } //!< Return the window position
			/*!< \return a const glm::ivec2 - The position of the window */
	};
}
#endif