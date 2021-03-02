/*! \file entity.h
*
* \brief An entity class which represents a gameobject in the game world. Attach entity components to describe the behaviour of the entity.
*
* \author Daniel Bullin
*
*/
#ifndef ENTITY_H
#define ENTITY_H

#include "independent/core/common.h"
#include "independent/core/destroyable.h"
#include "independent/events/events.h"

#include "independent/entities/components/camera.h"
#include "independent/entities/components/transform.h"
#include "independent/entities/components/eventListener.h"
#include "independent/entities/components/meshRender3D.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/characterController.h"
#include "independent/entities/components/light.h"

#include "independent/systems/systems/log.h"

namespace Engine
{
	class Scene; //!< Forward declare scene
	class Layer; //!< Forward declare layer

	/*! \class Entity
	* \brief An entity class which represents an individual existance in the game world
	*/
	class Entity : public Destroyable
	{
	private:
		std::string m_entityName; //!< The name of the entity
		Scene* m_parentScene; //!< The scene this entity belongs to
		Layer* m_layer; //!< The layer this entity is attached to
		std::vector<EntityComponent*> m_components; //!< List of components attached to this entity
		bool m_display; //!< Should the entity be displayed
	public:
		Entity(); //!< Constructor
		virtual ~Entity(); //!< Destructor

		const std::string& getName(); //!< Get the name of the entity
		void setName(const std::string& entityName); //!< Set the name of the entity

		void setParentScene(Scene* parent); //!< Set the scene this entity belongs to
		Scene* getParentScene() const; //!< Get the scene this entity belongs to

		void setLayer(Layer* layer); //!< Set the layer this entity belongs to
		Layer* getLayer() const; //!< Get the layer this entity belongs to

		void setDisplay(const bool display); //!< Set whether the entity should be displayed
		const bool getDisplay() const; //!< Get whether the entity should be displayed

		bool containsPoint(const glm::vec2& coordinate); //!< Returns whether the screen point is inside this entity's bounding box

		const std::vector<EntityComponent*>& getAllComponents(); //!< Get list of all components
		template<typename T, typename ...Args> void attach(const std::string& componentName, Args&&... args); //!< Attach a component to the entity
		template<typename T> void detach(); //!< Delete a component from the entity
		void detach(EntityComponent* component); //!< Delete a component from the entity
		template<typename T> T* getComponent(); //!< Get first instance of component by template type
		template<typename T> bool containsComponent(); //!< Check if the entity has a type of component attached

		virtual void onPreUpdate(const float timestep, const float totalTime) {} //!< Call before game update
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */
		virtual void onPostUpdate(const float timestep, const float totalTime) {} //!< Call after game update
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
		virtual void onRender(const Renderers renderer); //!< Call upon render if mesh render component attached
		virtual void printEntityDetails(); //!< Print entity details
	};

	template<typename T, typename ...Args>
	//! attach()
	/*!
	\param componentName a const std::string& - The name of the component
	\param args a Args&& - Parameter pack
	*/
	void Entity::attach(const std::string& componentName, Args&&... args)
	{
		if (!containsComponent<T>())
		{
			T* component = new T(std::forward<Args>(args) ...);
			m_components.emplace_back(component);
			m_components.back()->setName(componentName);
			m_components.back()->setParent(this);
			m_components.back()->onAttach();
		}
		else
			ENGINE_ERROR("[Entity::attach] Cannot add component as entity already this component attached. Entity Name: {0}, Component: {1}", m_entityName, componentName);
	}

	template<typename T>
	//! detach()
	void Entity::detach()
	{
		EntityComponent* component = getComponent<T>();
		detach(component);
	}

	template<typename T>
	//! getComponent()
	/*!
	\return a T* - A pointer to the component
	*/
	T* Entity::getComponent()
	{
		// Go through each component and check its type
		for (auto& component : m_components)
		{
			if (component)
			{
				if (typeid(*component) == typeid(T))
					return static_cast<T*>(component);
			}
		}
		return nullptr;
	}

	template<typename T>
	//! containsComponent()
	/*!
	\return a bool - Does this entity have the type of component
	*/
	bool Entity::containsComponent()
	{
		// Go through each component and check its type
		for (auto& component : m_components)
		{
			if (component)
			{
				if (typeid(*component) == typeid(T))
					return true;
			}
		}
		return false;
	}
}
#endif