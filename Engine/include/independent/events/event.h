/*! \file event.h 
*
* \brief Event class which stores all different event types
*
* \author DMU Course material
*
*/
#ifndef EVENT_H
#define EVENT_H

#include <inttypes.h>

namespace Engine {
	/*! \enum EventType
	* \brief An enum for all different event types
	*/
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MousePressed, MouseReleased, MouseMoved, MouseScrolled
	};

	/*! \enum EventCategory
	* \brief Enum for event category flags
	*/
	enum EventCategory
	{
		None = 0,
		EventCategoryWindow = 1 << 0,		// 00000001
		EventCategoryInput = 1 << 1,		// 00000010
		EventCategoryKeyboard = 1 << 2,		// 00000100
		EventCategoryMouse = 1 << 3,		// 00001000
		EventCategoryMouseButton = 1 << 4	// 00010000
	};

	/*! \class Event
	* \brief Event base class, all possible events that can arise will share this base class
	*/
	class Event 
	{
	protected:
		bool m_handled = false; //!< Has the event been handled
	public:
		virtual const EventType getEventType() const = 0; //!< Gets the event type
			/*!< \return a const EventType - The event type */
		virtual const int32_t getCategoryFlags() const = 0; //!< Gets the event category flags
			/*!< \return a const int32_t - The event's category flags */
		inline const bool isHandled() const { return m_handled; } //!< Has the event been handled
			/*!< \return a const bool - Has the event been handled */
		inline void handle(const bool isHandled) { m_handled = isHandled; } //!< Handle the event
			/*!< \param isHandled a const bool - Has the event been handled */
		inline const bool isInCategory(const EventCategory category) const { return getCategoryFlags() & category; } //!< Is the event in category
			/*!< \param category a const EventCategory - The event category
				 \return a const bool - Is the event a part of the event category */
	};
}
#endif