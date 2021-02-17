/*! \file entityComponent.cpp
*
* \brief An entity component which describes one particular feature of an entity. Follows the ECS architectural pattern
*
* \author Daniel Bullin
*
*/
#include "independent/entities/entityComponent.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! EntityComponent()
	/*!
	\param type a const ComponentType - The type of component
	*/
	EntityComponent::EntityComponent(const ComponentType type) : m_componentType(type), m_parentEntity(0)
	{
	}

	//! ~EntityComponent()
	EntityComponent::~EntityComponent()
	{
		ENGINE_INFO("[EntityComponent::~EntityComponent] Deleting entity component: {0} attached to entity: {1}.", m_name, m_parentEntity->getName());
	}

	//! setParent()
	/*!
	\param parent an Entity* - A pointer to the entity this component is attached to
	*/
	void EntityComponent::setParent(Entity* parent)
	{
		if (parent)
			m_parentEntity = parent;
		else
			ENGINE_ERROR("[EntityComponent::setParent] Entity parent is a null pointer. Component Name: {0}.", m_name);
	}

	//! getParent()
	/*!
	\return an Entity* - A pointer to the entity this component is attached to
	*/
	Entity* EntityComponent::getParent()
	{
		return m_parentEntity;
	}

	//! setName()
	/*!
	\param name a const char* - The name of the component
	*/
	void EntityComponent::setName(const char* name)
	{
		if (name)
			m_name = name;
	}

	//! getName()
	/*!
	\return a const std::string& - The name of the component
	*/
	const std::string& EntityComponent::getName() const
	{
		return m_name;
	}

	//! getComponentType()
	/*!
	\return a const ComponentType& - The component type
	*/
	const ComponentType& EntityComponent::getComponentType() const
	{
		return m_componentType;
	}

}