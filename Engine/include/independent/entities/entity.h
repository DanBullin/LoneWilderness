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
#include "independent/entities/components/meshRender3D.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/characterController.h"
#include "independent/entities/components/light.h"
#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/UIElement.h"

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
		Entity* m_parentEntity; //!< The parent entity
		Scene* m_parentScene; //!< The scene this entity belongs to
		Layer* m_layer; //!< The layer this entity is attached to
		std::map<std::string, Entity*> m_childEntities; //!< List of child entities
		std::vector<EntityComponent*> m_components; //!< List of components attached to this entity
		bool m_display; //!< Should the entity be displayed
		bool m_selected; //!< Is the entity selected
	public:
		Entity(); //!< Constructor
		virtual ~Entity(); //!< Destructor

		const std::string& getName(); //!< Get the name of the entity
		void setName(const std::string& entityName); //!< Set the name of the entity

		void onUpdate(const float timestep, const float totalTime); //!< Update the entity

		void setParentEntity(Entity* parent); //!< Set the entity this entity belongs to
		Entity* getParentEntity() const; //!< Get the entity this entity belongs to
		Entity* getChildEntity(const std::string& childName); //!< Get the child entity of this entity by name
		std::map<std::string, Entity*>& getChildEntities(); //!< Get all the children
		void getAllEntities(std::vector<Entity*>& entityList); //!< Get all entities belonging to this entity

		void setParentScene(Scene* parent); //!< Set the scene this entity belongs to
		Scene* getParentScene() const; //!< Get the scene this entity belongs to

		void setLayer(Layer* layer); //!< Set the layer this entity belongs to
		Layer* getLayer() const; //!< Get the layer this entity belongs to

		void setDisplay(const bool display); //!< Set whether the entity should be displayed
		const bool getDisplay() const; //!< Get whether the entity should be displayed

		bool containsPoint(const glm::vec2& coordinate); //!< Returns whether the screen point is inside this entity's bounding box

		bool addChildEntity(const std::string& childName, Entity* entity); //!< Add a child entity
		bool checkChildEntityNameTaken(const std::string& name) const; //!< Returns whether the entity name has been taken

		void setSelected(bool select);
		bool getSelected();

		std::vector<EntityComponent*>& getAllComponents(); //!< Get list of all components
		template<typename T> void attach(EntityComponent* component); //!< Attach an already created component
		template<typename T, typename ...Args> void attach(const std::string& componentName, Args&&... args); //!< Attach a component to the entity
		template<typename T> void detach(); //!< Delete a component from the entity
		void detach(EntityComponent* component); //!< Delete a component from the entity
		void detachDelayDeletion(EntityComponent* component); //!< Delete a component from the entity
		template<typename T> T* getComponent(); //!< Get first instance of component by template type
		template<typename T> bool containsComponent(); //!< Check if the entity has a type of component attached

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
	//! attach()
	/*
	\param component an EntityComponent* - A pointer to the component to attach
	*/
	void Entity::attach(EntityComponent* component)
	{
		if (!containsComponent<T>())
		{
			if (component)
			{
				m_components.emplace_back(component);
				m_components.back()->setParent(this);
				m_components.back()->onAttach();
			}
			else
				ENGINE_ERROR("[Entity::attach] Cannot add component as component isn't valid. Entity Name: {0}.", m_entityName);
		}
		else
		{
			ENGINE_ERROR("[Entity::attach] Cannot add component as entity already this component attached. Entity Name: {0}", m_entityName);

			if (component)
				delete component;
		}
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
				if (typeid(*component) == typeid(T) || component->getComponentType() == Components::toType(typeid(T).name()))
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
				if (typeid(*component) == typeid(T) || component->getComponentType() == Components::toType(typeid(T).name()))
					return true;
			}
		}
		return false;
	}
}
#endif