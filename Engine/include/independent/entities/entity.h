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
#include "independent/entities/components/camera.h"
#include "independent/entities/components/transform3D.h"
#include "independent/entities/components/transform2D.h"
#include "independent/entities/components/eventListener.h"
#include "independent/entities/components/meshRender3D.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"
#include "independent/events/events.h"

namespace Engine
{
	class Scene; //!< Forward declare scene
	class Layer; //!< Forward declare layer

	/*! \class Entity
	* \brief An entity class which represents an individual existance in the game world
	*/
	class Entity
	{
	private:
		std::string m_entityName; //!< The name of the entity
		Scene* m_parentScene; //!< The scene this entity belongs to
		Layer* m_layer; //!< The layer this entity is attached to

		std::map<std::string, EntityComponent*> m_components; //!< List of components attached to this entity

		bool m_delete; //!< Delete this entity at the end of the frame

		const ComponentType convertClassType(const std::string& classType); //!< Get component type from class type
	public:
		Entity(); //!< Constructor
		virtual ~Entity(); //!< Destructor

		const std::string& getName(); //!< Get the name of the entity
		void setName(const char* entityName); //!< Set the name of the entity

		void setParentScene(Scene* parent); //!< Set the scene this entity belongs to
		Scene* getParentScene() const; //!< Get the scene this entity belongs to

		void setLayer(Layer* layer); //!< Set the layer this entity belongs to
		Layer* getLayer() const; //!< Get the layer this entity belongs to

		void setDelete(const bool deletion); //!< Set the schedule entity deletion
		const bool getDelete() const; //!< Get the schedule entity deletion

		void detach(const char* name); //!< Delete a component from the entity
		std::map<std::string, EntityComponent*>& getAllComponents(); //!< Get list of all components
		template<typename T, typename ...Args> void attach(const char* componentName, Args&&... args); //!< Attach a component to the entity
		template<typename T> T* getComponent(); //!< Get first instance of component by template type
		template<typename T> T* getComponent(const char* componentName); //!< Get a component by name
		template<typename T> std::vector<T*> getComponents(); //!< Get all components of type
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
		virtual void onRender(const Renderers::Renderers renderer); //!< Call upon render if mesh render component attached

		virtual void printEntityDetails(); //!< Print entity details
	};

	template<typename T, typename ...Args>
	//! attach()
	/*!
	\param componentName a const char* - The name of the component
	\param args a Args&& - Parameter pack
	*/
	void Entity::attach(const char* componentName, Args&&... args)
	{
		// Check if component name is free
		if (m_components.find(componentName) == m_components.end())
		{
			EntityComponent* component;
			uint32_t limit = Components::getComponentLimit(typeid(T).name());

			if (getComponents<T>().size() < limit)
			{
				component = new T(std::forward<Args>(args) ...);

				m_components[componentName] = component;
				m_components[componentName]->setParent(this);
				m_components[componentName]->setName(componentName);
				m_components[componentName]->onAttach();
			}
			else
				ENGINE_ERROR("[Entity::attach] Cannot add component as you have reached the limit of how many components of this type can be added to one entity. Entity Name: {0}, Component: {1}", m_entityName, componentName);
		}
		else
			ENGINE_ERROR("[Entity::attach] Cannot add component as name already exists. Entity Name: {0}, Component: {1}", m_entityName, componentName);
	}

	template<typename T>
	//! getComponent()
	/*!
	\return a T* - A pointer to the component
	*/
	T* Entity::getComponent()
	{
		ComponentType type = convertClassType(typeid(T).name());

		// Get first (and should be only) type of component
		for (auto& comp : m_components)
		{
			if (comp.second->getComponentType() == type) 
				return static_cast<T*>(comp.second);
		}

		// Cannot find component by type
		return nullptr;
	}

	template<typename T>
	//! getComponent()
	/*!
	\param componentName a const char* - The name of the component
	\return a T* - A pointer to the component
	*/
	T* Entity::getComponent(const char* componentName)
	{
		// Find component by name
		if (m_components.find(componentName) != m_components.end())
		{
			if (convertClassType(typeid(T).name()) == m_components[componentName]->getComponentType())
				return static_cast<T*>(m_components[componentName]);
			else
				ENGINE_ERROR("[Entity::getComponent] An incorrect template type was provided for the component: {0}.", componentName);
		}
		else
			ENGINE_ERROR("[Entity::getComponent] Name for component wasn't found in the list. Entity Name: {0}, Component Name: {1}.", m_entityName, componentName);

		return nullptr;
	}

	template<typename T>
	//! getComponents()
	/*!
	\return a std::vector<T*> - A list of all components of type provided
	*/
	std::vector<T*> Entity::getComponents()
	{
		std::vector<T*> list;
		ComponentType type = convertClassType(typeid(T).name());

		// Go through each component and check its type
		for (auto& component : m_components)
		{
			// If type matches the type we're looking for, add it to the list
			if (component.second->getComponentType() == type)
				list.push_back(static_cast<T*>(component.second));
		}
		return list;
	}

	template<typename T>
	//! containsComponent()
	/*!
	\return a bool - Is the component type found in the entity
	*/
	bool Entity::containsComponent()
	{
		ComponentType type = convertClassType(typeid(T).name());

		// Go through each component and check its type
		for (auto& component : m_components)
		{
			if (component.second->getComponentType() == type)
				return true;
		}
		return false;
	}
}
#endif