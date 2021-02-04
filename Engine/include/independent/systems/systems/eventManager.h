/*! \file eventManager.h
*
* \brief An event manager which manages events
*
* \author Daniel Bullin
*
*/
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "independent/core/common.h"
#include "independent/systems/system.h"
#include "independent/events/events.h"
#include "independent/systems/components/window.h"

namespace Engine {
	class Entity; //!< Forward declare entity

	/*! \struct EventData
	* \brief Struct containing event related data such as mouse position last frame, etc
	*/
	struct EventData
	{
		float lastMousePosX; //!< Mouse x position last frame
		float lastMousePosY; //!< Mouse y position last frame
		glm::vec2 mouseOffset; //!< The change in mouse position since last frame
		bool firstMouse = true; //!< Initial mouse capture?
		float deltaTime = 0.0f; //!< Used to measure time
		float lastFrame = 0.0f; //!< Time to render last frame
	};

	/*! \class EventManager
	* \brief An event manager which manages events
	*/
	class EventManager : public System {
	private:
		static bool s_enabled; //!< Is the event manager enabled
		static EventData s_eventData; //!< Event related variables
		static void calculateMouseOffset(MouseMovedEvent& e); //!< Calculate change in mouse position when moved
		static void updateTime(); //!< Update FPS and TotalTime
	public:
		EventManager(); //!< Constructor
		~EventManager(); //!< Destructor
		virtual void start() override; //!< Start the system
		virtual void stop() override; //!< Stop the system

		static void onWindowClose(Window* window, WindowCloseEvent& e); //!< Called when a window closes
		static void onWindowResize(Window* window, WindowResizeEvent& e); //!< Called when a window is resized
		static void onWindowFocus(Window* window, WindowFocusEvent& e); //!< Called when a window gains focus
		static void onWindowLostFocus(Window* window, WindowLostFocusEvent& e); //!< Called when a window loses focus
		static void onWindowMoved(Window* window, WindowMovedEvent& e); //!< Called when a window is moved
		static void onKeyPressed(Window* window, KeyPressedEvent& e); //!< Called when a key is pressed or held
		static void onKeyReleased(Window* window, KeyReleasedEvent& e); //!< Called when a key is released
		static void onKeyTyped(Window* window, KeyTypedEvent& e); //!< Called when a key is typed
		static void onMousePressed(Window* window, MousePressedEvent& e); //!< Called when mouse is pressed
		static void onMouseReleased(Window* window, MouseReleasedEvent& e); //!< Called when mouse is released
		static void onMouseMoved(Window* window, MouseMovedEvent& e); //!< Called when mouse moves
		static void onMouseScrolled(Window* window, MouseScrolledEvent& e); //!< Called when mouse is scrolled

		static void onUpdate(const float timestep, const float totalTime); //!< Called once every frame

		static void enable(); //!< Enable the event manager
		static void disable(); //!< Disable the event manager
		static const bool getEnabled(); //!< Get whether the event manager is enabled
		static EventData& getEventData(); //!< Get the event data
		static void printEventData(); //!< Print event data
	};
}
#endif