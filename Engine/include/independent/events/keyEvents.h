/*! \file keyEvents.h 
*
* \brief The types of key events
*
* \author DMU Course material
*
*/
#ifndef KEYEVENTS_H
#define KEYEVENTS_H

#include "independent/events/event.h"

namespace Engine {
	/*! \class KeyEvent
	* \brief A base event class for key events
	*/
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(const int32_t keycode) : m_keyCode(keycode) {} //!< Constructor
			/*!< \param keycode a const int32_t - The key involved in the event */
		int32_t m_keyCode; //!< The key involved in the event
	public:
		inline const int32_t getKeyCode() const { return m_keyCode; } //!< Get the key involved in the event
			/*!< \return a const int32_t - The key involved in the event */
		virtual inline const int32_t getCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; } //!< Get the category flags of the event
			/*!< \return a const int32_t - The key's event category flags */
	};

	/*! \class KeyPressedEvent
	* \brief A key pressed event
	*/
	class KeyPressedEvent : public KeyEvent {
	private:
		int32_t m_repeatCount; //!< Is the key press event a repeat press
	public:
		KeyPressedEvent(const int32_t keycode, const int32_t repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {} //!< Constructor
			/*!< \param keycode a const int32_t - The key involved in the event 
				 \param repeatCount a const int32_t - Is the key press a repeat event */
		inline const int32_t getRepeatCount() const { return m_repeatCount; } //!< Returns whether the event is a repeat
			/*!< \return a const int32_t - The repeat count */
		static inline const EventType getStaticType() { return EventType::KeyPressed; } //!< Return static type
			/*!< \return a const EventType - The static key press event type */
		virtual inline const EventType getEventType() const override { return EventType::KeyPressed; } //!< Gets the event type
			/*!< \return a const EventType - The key press event type */
	};

	/*! \class KeyReleasedEvent
	* \brief A key released event
	*/
	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(const int32_t keycode) : KeyEvent(keycode) {} //!< Constructor
			/*!< \param keycode a const int32_t - The key involved in the event */
		static inline const EventType getStaticType() { return EventType::KeyReleased; } //!< Return static type
			/*!< \return a const EventType - The static key release event type */
		virtual inline const EventType getEventType() const override { return EventType::KeyReleased; } //!< Gets the event type
			/*!< \return a const EventType - The key release event type */
	};

	/*! \class KeyTypedEvent
	* \brief A key typed event
	*/
	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(const int32_t keycode) : KeyEvent(keycode) {} //!< Constructor
			/*!< \param keycode a const int32_t - The key involved in the event */
		static inline const EventType getStaticType() { return EventType::KeyTyped; } //!< Return static type
			/*!< \return a const EventType - The static key typed event type */
		virtual inline const EventType getEventType() const override { return EventType::KeyTyped; } //!< Gets the event type
			/*!< \return a const EventType - The key typed event type */
	};
}
#endif