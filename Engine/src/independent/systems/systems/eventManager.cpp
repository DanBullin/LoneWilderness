/*! \file eventManager.cpp
*
* \brief An event manager which manages events
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/eventManager.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/timerSystem.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/threadManager.h"
#include "independent/entities/entity.h"

namespace Engine
{
	bool EventManager::s_enabled = false; //!< Initialise with default value of false
	EventData EventManager::s_eventData = EventData(); //!< Initialise with default constructor
	Scene* EventManager::s_currentScene = nullptr; //!< Initialise with null pointer

	//! calculateMouseOffset()
	/*!
	\param e a MouseMovedEvent& - Reference to the mouse moved event
	*/
	void EventManager::calculateMouseOffset(MouseMovedEvent& e)
	{
		if (s_enabled)
		{
			// Update event data and mouse position change
			if (s_eventData.firstMouse)
			{
				s_eventData.lastMousePosX = e.getX();
				s_eventData.lastMousePosY = e.getY();
				s_eventData.firstMouse = false;
			}

			// Calculate the offset of the current position from the position last frame
			s_eventData.mouseOffset.x = e.getX() - s_eventData.lastMousePosX;
			s_eventData.mouseOffset.y = s_eventData.lastMousePosY - e.getY(); // Reversed since y-coordinates go from bottom to top

			// Set current position as last position in preparation for the next update
			s_eventData.lastMousePosX = e.getX();
			s_eventData.lastMousePosY = e.getY();
		}
	}

	//! EventManager()
	EventManager::EventManager() : System(SystemType::EventManager)
	{
	}

	//! ~EventManager()
	EventManager::~EventManager()
	{
	}

	//! start()
	void EventManager::start()
	{
		// Start the system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[EventManager::start] Starting the event manager system.");
			s_enabled = true;
		}
	}

	//! stop()
	void EventManager::stop()
	{
		// Stop the system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[EventManager::stop] Stopping the event manager system.");
			s_enabled = false;
		}
	}

	//! onWindowClose()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a WindowCloseEvent& - Reference to the window close event
	*/
	void EventManager::onWindowClose(Window* window, WindowCloseEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			// Schedule this window for deletion
			window->onWindowClose(e);
		}
	}

	//! onWindowResize()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a WindowResizeEvent& - Reference to the window resize event
	*/
	void EventManager::onWindowResize(Window* window, WindowResizeEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			window->onWindowResize(e);
			s_currentScene->getMainCamera()->updateProjection(window->getProperties().getSizef());

			// Pass input event to layers and entities 
			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onWindowResize(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onWindowResize(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
			else
				ENGINE_ERROR("[EventManager::onWindowResize] Cannot get the active scene.");
		}
	}

	//! onWindowFocus()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a WindowFocusEvent& - Reference to the window focus event
	*/
	void EventManager::onWindowFocus(Window* window, WindowFocusEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			// Set this window as the focused window
			window->onWindowFocus(e);
			WindowManager::setFocusedWindowByName(window->getName().c_str());

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onWindowFocus(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onWindowFocus(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onWindowLostFocus()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a WindowLostFocusEvent& - Reference to the window lost focus event
	*/
	void EventManager::onWindowLostFocus(Window* window, WindowLostFocusEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			window->onWindowLostFocus(e);

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onWindowLostFocus(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onWindowLostFocus(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onWindowMoved()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a WindowMovedEvent& - Reference to the window moved event
	*/
	void EventManager::onWindowMoved(Window* window, WindowMovedEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			window->onWindowMoved(e);

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onWindowMoved(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onWindowMoved(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onKeyPressed()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a KeyPressedEvent& - Reference to the key pressed event
	*/
	void EventManager::onKeyPressed(Window* window, KeyPressedEvent& e)
	{
		if (s_enabled && e.getKeyCode() != -1)
		{
			// Check window is valid
			if (!window) return;

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onKeyPress(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onKeyPress(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onKeyReleased()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a KeyReleasedEvent& - Reference to the key released event
	*/
	void EventManager::onKeyReleased(Window* window, KeyReleasedEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onKeyRelease(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onKeyRelease(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}

			if (e.getKeyCode() == Keys::ESC)
			{
				if (SceneManager::getScene("mainMenu"))
				{
					SceneManager::setActiveScene("mainMenu", true);
					window->setCursorInputMode(CursorInputMode::Visible);
				}
				else ENGINE_ERROR("No main menu to return to!");
			}
		}
	}

	//! onKeyTyped()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a KeyTypedEvent& - Reference to the key typed event
	*/
	void EventManager::onKeyTyped(Window* window, KeyTypedEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;
		}
	}

	//! onMousePressed()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a MousePressedEvent& - Reference to the mouse pressed event
	*/
	void EventManager::onMousePressed(Window* window, MousePressedEvent& e)
	{
		if (s_enabled && e.getButton() != -1)
		{
			// Check window is valid
			if (!window) return;

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onMousePress(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onMousePress(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onMouseReleased()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a MouseReleasedEvent& - Reference to the mouse released event
	*/
	void EventManager::onMouseReleased(Window* window, MouseReleasedEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onMouseRelease(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onMouseRelease(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onMouseMoved()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a MouseMovedEvent& - Reference to the mouse moved event
	*/
	void EventManager::onMouseMoved(Window* window, MouseMovedEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			// Update event data and mouse position change
			if (window == WindowManager::getFocusedWindow()) calculateMouseOffset(e);

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onMouseMoved(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onMouseMoved(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onMouseScrolled()
	/*!
	\param window a Window* - A pointer to the window which triggered the event
	\param e a MouseScrolledEvent& - Reference to the mouse scrolled event
	*/
	void EventManager::onMouseScrolled(Window* window, MouseScrolledEvent& e)
	{
		if (s_enabled)
		{
			// Check window is valid
			if (!window) return;

			if (s_currentScene)
			{
				if (s_currentScene->getLayerManager())
				{
					for (auto& layer : s_currentScene->getLayerManager()->getLayers())
					{
						if (layer)
						{
							if (layer->getActive())
								layer->onMouseScrolled(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
						}
					}
				}

				for (auto& entity : s_currentScene->getEntities())
				{
					if (entity)
					{
						if (entity->getLayer()->getActive() && entity->containsComponent<NativeScript>())
							entity->getComponent<NativeScript>()->onMouseScrolled(e, TimerSystem::getStoredTime("FPS"), TimerSystem::getStoredTime("TotalTime"));
					}
				}
			}
		}
	}

	//! onUpdate()
	/*!
	\param scene a Scene* - A pointer to the scene to be sent updates
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void EventManager::onUpdate(Scene* scene, const float timestep, const float totalTime)
	{
		if (s_enabled)
		{
			// Check if scene is valid
			if (scene) s_currentScene = scene;

			// Update FPS and total time
			updateTime();

			Window* window = WindowManager::getFocusedWindow();
			if (!window)
			{
				ENGINE_ERROR("[EventManager::onUpdate] Invalid focused window.");
				return;
			}

			// Call entity preupdate
			for (auto& entity : s_currentScene->getEntities())
			{
				if (entity)
				{
					if (entity->containsComponent<NativeScript>())
						entity->getComponent<NativeScript>()->onPreUpdate(timestep, totalTime);
				}
			}

			// Use input poller to check for input and send event
			for (auto key : InputPoller::isAnyKeyPressed())
			{
				KeyPressedEvent keyE(key, 1);
				onKeyPressed(window, keyE);
			}

			for (auto button : InputPoller::isAnyMouseButtonPressed())
			{
				MousePressedEvent mouseE(button);
				onMousePressed(window, mouseE);
			}

			// Update Active scene
			if (s_currentScene) s_currentScene->onUpdate(timestep, totalTime);

			ThreadManager::onUpdate(timestep, totalTime);

			// Update all registered windows
			for (auto& window : WindowManager::getRegisteredWindows())
			{
				if (window.second)
					window.second->onUpdate(timestep, totalTime);
			}

			// Call entity postupdate
			for (auto& entity : s_currentScene->getEntities())
			{
				if (entity)
				{
					if (entity->containsComponent<NativeScript>())
						entity->getComponent<NativeScript>()->onPostUpdate(timestep, totalTime);
				}
			}
		}
	}

	//! enable()
	void EventManager::enable()
	{
		s_enabled = true;
	}

	//! disable()
	void EventManager::disable()
	{
		s_enabled = false;
	}

	//! getEnabled()
	/*!
	\return a const bool - Is the event manager enabled
	*/
	const bool EventManager::getEnabled()
	{
		return s_enabled;
	}

	//! getEventData()
	/*!
	\return an EventData& - The event data
	*/
	EventData& EventManager::getEventData()
	{
		return s_eventData;
	}

	//! printEventData()
	void EventManager::printEventData()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("Event Data");
			ENGINE_TRACE("===========");
			ENGINE_TRACE("Last Mouse Position: {0}, {1}", s_eventData.lastMousePosX, s_eventData.lastMousePosY);
			ENGINE_TRACE("Mouse Change: {0}, {1}", s_eventData.mouseOffset.x, s_eventData.mouseOffset.y);
			ENGINE_TRACE("First Mouse: {0}", s_eventData.firstMouse);
			ENGINE_TRACE("Delta time: {0}", s_eventData.deltaTime);
			ENGINE_TRACE("Last frame time: {0}", s_eventData.lastFrame);
			ENGINE_TRACE("===========");
		}
		else
			ENGINE_ERROR("[EventManager::printEventData] This system has not been enabled.");
	}

	//! updateTime
	void EventManager::updateTime()
	{
		if (s_enabled)
		{
			// Update time since last frame
			s_eventData.deltaTime = TimerSystem::getStoredTime("FPS") - s_eventData.lastFrame;
			s_eventData.lastFrame = TimerSystem::getStoredTime("FPS");
		}
	}
}