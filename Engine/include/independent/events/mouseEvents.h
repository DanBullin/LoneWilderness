/*! \file mouseEvents.h 
*
* \brief The types of mouse events
*
* \author DMU Course material
*
*/
#ifndef MOUSEEVENTS_H
#define MOUSEEVENTS_H

#include "independent/events/event.h"
#include <glm/glm.hpp>

namespace Engine {
	/*! \class MouseEvent
	* \brief A base event class for mouse events
	*/
	class MouseEvent : public Event {
	public:
		virtual inline const int32_t getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
			/*!< \return a const int32_t - The mouse's event category flags */
	};

	/*! \class MouseMovedEvent
	* \brief A mouse moved event
	*/
	class MouseMovedEvent : public MouseEvent {
	private:
		float m_mousePosX; //!<	x Position of the mouse
		float m_mousePosY; //!< y Position of the mouse
	public:
		MouseMovedEvent(const float x, const float y) : m_mousePosX(x), m_mousePosY(y) {} //!< Constructor
			/*!< \param x a const float - The x position of the mouse
				 \param y a const float - The y position of the mouse */
		static inline const EventType getStaticType() { return EventType::MouseMoved; } //!< Return static type
			/*!< \return a const EventType - The static mouse moved event type */
		virtual inline const EventType getEventType() const override { return EventType::MouseMoved; } //!< Gets the event type
			/*!< \return a const EventType - The mouse moved event type */
		inline const float getX() const { return m_mousePosX; } //!< Return the x position of the mouse
			/*!< \return a const float - The x position of the mouse */
		inline const float getY() const { return m_mousePosY; } //!< Return the y position of the mouse
			/*!< \return a const float - The y position of the mouse */
		inline const glm::vec2 getPos() const { return { m_mousePosX, m_mousePosY }; } //!< Return the position of the mouse
			/*!< \return a const glm::vec2 - The position of the mouse */
	};

	/*! \class MouseScrolledEvent
	* \brief A mouse scrolled event
	*/
	class MouseScrolledEvent : public MouseEvent {
	private:
		float m_xOffset; //!< The x offset of the mouse wheel
		float m_yOffset; //!< The y offset of the mouse wheel
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {} //!< Constructor
			/*!< \param xOffset a const float - The x offset of the mouse wheel
				 \param yOffset a const float - The y offset of the mouse wheel */
		static inline const EventType getStaticType() { return EventType::MouseScrolled; } //!< Return static type
			/*!< \return a const EventType - The static mouse scrolled event type */
		virtual inline const EventType getEventType() const override { return EventType::MouseScrolled; } //!< Gets the event type
			/*!< \return a const EventType - The mouse scrolled event type */
		inline const float getXOffset() const { return m_xOffset; }
			/*!< \return a const float - The x offset of the mouse wheel */
		inline const float getYOffset() const { return m_yOffset; }
			/*!< \return a const float - The y offset of the mouse wheel */
	};

	/*! \class MousePressedEvent
	* \brief A mouse pressed event
	*/
	class MousePressedEvent : public MouseEvent {
	private:
		int32_t m_button; //!< The button pressed
	public:
		MousePressedEvent(const int32_t button) : m_button(button) {} //!< Constructor
			/*!< \param button a const int32_t - The mouse button pressed */
		static inline const EventType getStaticType() { return EventType::MousePressed; } //!< Return static type
			/*!< \return a const EventType - The static mouse pressed event type */
		virtual inline const EventType getEventType() const override { return EventType::MousePressed; } //!< Gets the event type
			/*!< \return a const EventType - The mouse pressed event type */
		inline const int32_t getButton() const { return m_button; }
			/*!< \return a const int32_t - The mouse button pressed */
	};

	/*! \class MouseReleasedEvent
	* \brief A mouse released event
	*/
	class MouseReleasedEvent : public MouseEvent {
	private:
		int32_t m_button; //!< The button released
	public:
		MouseReleasedEvent(const int32_t button) : m_button(button) {} //!< Constructor
			/*!< \param button a const int32_t - The mouse button released */
		static inline const EventType getStaticType() { return EventType::MouseReleased; } //!< Return static type
			/*!< \return a const EventType - The static mouse released event type */
		virtual inline const EventType getEventType() const override { return EventType::MouseReleased; } //!< Gets the event type
			/*!< \return a const EventType - The mouse released event type */
		inline const int32_t getButton() const { return m_button; }
			/*!< \return a const int32_t - The mouse button pressed */
	};
}
#endif