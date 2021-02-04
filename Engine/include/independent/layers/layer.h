/*! \file layer.h
*
* \brief A layer containing a group of scene data
*
* \author Daniel Bullin
*
*/
#ifndef LAYER_H
#define LAYER_H

#include "independent/core/common.h"
#include "independent/entities/entity.h"

namespace Engine 
{
	class LayerManager; //!< Forward declare

	/*! \class Layer
	* \brief A layer containing a set of scene data
	*/
	class Layer 
	{
	private:
		std::string m_layerName; //!< The name of the layer
		bool m_display; //!< Should this layer be rendered
		bool m_active; //!< Should this layer be receiving updates
		LayerManager* m_layerManager; //!< The layer manager this scene is attached to
	public:
		Layer(const char* layerName); //!< Constructor
		virtual ~Layer(); //!< Destructor

		virtual void onAttach() {} //!< Called when attaching to layer manager
		virtual void onDetach() {} //!< Called when detaching from layer manager
		virtual void onUpdate(const float timestep, const float totalTime) {} //!< Called one per frame
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */
		virtual void onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime) {} //!< Called when a window is resized
			/* \param e a WindowResizeEvent& - Reference to the window resize event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime) {} //!< Called when a window gains focus
			/* \param e a WindowFocusEvent& - Reference to the window focus event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime) {} //!< Called when a window loses focus
			/* \param e a WindowLostFocusEvent& - Reference to the window lost focus event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime) {} //!< Called when a window is moved
			/* \param e a WindowMovedEvent& - Reference to the window moved event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) {} //!< Call upon key press
			/* \param e a KeyPressedEvent& - Reference to the key pressed event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) {} //!< Call upon key release
			/* \param e a onKeyRelease& - Reference to the key release event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse press
			/* \param e a MousePressedEvent& - Reference to the mouse press event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse release
			/* \param e a MouseReleasedEvent& - Reference to the mouse release event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse scroll
			/* \param e a MouseScrolledEvent& - Reference to the mouse scroll event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse move
			/* \param e a MouseMovedEvent& - Reference to the mouse move event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */

		void printLayerDetails(); //!< Print the layer details

		const std::string& getLayerName(); //!< Get the name of the layer
		const bool getDisplayed() const; //!< Get whether this layer should be rendered
		void setDisplayed(const bool display); //!< Set whether this layer should be rendered
		const bool getActive() const; //!< Get whether this layer is active and receiving events
		void setActive(const bool active); //!< Set whether this layer is active and receiving events

		void setLayerManager(LayerManager* manager); //!< Set the layer manager
		LayerManager* getLayerManager() const; //!< Get the layer manager

		std::vector<Entity*> getEntities(); //!< Get all entities that are on this layer
		std::vector<Entity*> getRenderableEntities(); //!< Get all entities that contain some rendering component on this layer
	};
}
#endif