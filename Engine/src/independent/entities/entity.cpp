/*! \file entity.cpp
*
* \brief An entity class which represents a gameobject in the game world. Attach entity components to describe the behaviour of the entity
*
* \author Daniel Bullin
*
*/
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/components/scene.h"
#include "independent/systems/systems/renderSystem.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//! Entity()
	Entity::Entity()
	{
		m_entityName = "";
		m_parentScene = nullptr;
		m_parentEntity = nullptr;
		m_layer = nullptr;
		m_display = true;
		m_selected = false;
	}

	//! ~Entity()
	Entity::~Entity()
	{
		ENGINE_INFO("[Entity::~Entity] Deleting entity named: {0}", m_entityName);

		for (auto& comp : m_components)
		{
			if (comp)
			{
				comp->onDetach();
				delete comp;
			}
		}

		m_components.clear();

		for (auto& child : m_childEntities)
		{
			if (child.second)
			{
				delete child.second;
			}
		}

		m_childEntities.clear();

		m_parentScene = nullptr;
		m_layer = nullptr;
	}

	//! getName()
	/*!
	\return a const std::string& - The name of the entity
	*/
	const std::string& Entity::getName()
	{
		return m_entityName;
	}

	//! setName()
	/*!
	\param entityName a const std::string& - The name of the entity
	*/
	void Entity::setName(const std::string& entityName)
	{
		m_entityName = entityName;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The update time step
	\param totalTime a const float - The total application time
	*/
	void Entity::onUpdate(const float timestep, const float totalTime)
	{
		// Delete any child entities that are scheuled to be deleted, can also update entities if entity is not to be deleted
		for (auto it = m_childEntities.cbegin(); it != m_childEntities.cend(); )
		{
			// Check if entity is a valid entity
			if (it->second)
			{
				// Check if the entity is to be destroyed
				if (it->second->getDestroyed())
				{
					// Do NOT delete the entity which contains the main camera
					if (it->second == getParentScene()->getMainCamera()->getParent())
					{
						ENGINE_ERROR("[Entity::onUpdate] Cannot delete the entity when it contains the scene's main camera. Scene: {0}, Entity: {1}.", getParentScene()->getName(), it->second->getName());
						it->second->undestroy();
						return;
					}

					// Check for validness again and then delete
					if (it->second) delete it->second;
					m_childEntities.erase(it++);
					getParentScene()->setEntityListUpdated(true);
				}
				else
				{
					// Entity is not to be deleted, update components
					if (it->second)
					{
						it->second->onUpdate(timestep, totalTime);
						for (auto& component : it->second->getAllComponents())
						{
							// Check if component is valid
							if (component)
								component->onUpdate(timestep, totalTime);
						}
						++it;
					}
				}
			}
		}
	}

	//! setParentEntity()
	/*
	\param parent an Entity* - A pointer to the entity
	*/
	void Entity::setParentEntity(Entity* parent)
	{
		m_parentEntity = parent;
	}

	//! getParentEntity()
	/*
	\return an Entity* - A pointer to the parent entity
	*/
	Entity* Entity::getParentEntity() const
	{
		return m_parentEntity;
	}

	//! getChildEntity()
	/*
	\param childName a const std::string& - The name of the child entity
	\return an Entity* - A pointer to the child entity
	*/
	Entity* Entity::getChildEntity(const std::string& childName)
	{
		// Check if entity name exists
		if (checkChildEntityNameTaken(childName) && childName != "")
		{
			if (!m_childEntities[childName])
				ENGINE_ERROR("[Entity::getChildEntity] The entity we're retrieving is an invalid entity. Entity Name: {0}", getName());

			return m_childEntities[childName];
		}
		else
			ENGINE_ERROR("[Entity::getChildEntity] Cannot get child entity named: {0}.", childName);

		// Can't find entity
		return nullptr;
	}

	//! getChildEntities()
	/*
	\return a std::map<std::string, Entity*>& - The list of child entities
	*/
	std::map<std::string, Entity*>& Entity::getChildEntities()
	{
		return m_childEntities;
	}

	//! getAllEntities()
	/*
	\param entityList a std::vector<Entity*>& - The current list of entities
	*/
	void Entity::getAllEntities(std::vector<Entity*>& entityList)
	{
		for (auto& entity : getChildEntities())
		{
			if (entity.second)
			{
				entityList.push_back(entity.second);
				if (entity.second->getChildEntities().size() != 0)
				{
					entity.second->getAllEntities(entityList);
				}
			}
		}
	}

	//! setParentScene()
	/*!
	\param parent a Scene* - A pointer to the scene
	*/
	void Entity::setParentScene(Scene* parent)
	{
		if (parent)
			m_parentScene = parent;
		else
			ENGINE_ERROR("[Entity::setParentScene] New scene parent is a null pointer.");
	}

	//! getParentScene()
	/*!
	\return a Scene* - A pointer to the entity's parent scene
	*/
	Scene* Entity::getParentScene() const
	{
		if (!m_parentScene)
			ENGINE_ERROR("[Entity::getParentScene] Parent scene is a null pointer.");
		return m_parentScene;
	}

	//! setLayer()
	/*!
	\param layer a Layer* - A pointer to the layer
	*/
	void Entity::setLayer(Layer* layer)
	{
		if (layer)
		{
			Scene* scene = getParentScene();

			if (scene)
			{
				LayerManager* layerManager = scene->getLayerManager();
				if (layerManager)
				{
					// Check if new layer exists in the entity's parent scene layers
					if (layerManager->layerExists(layer))
						m_layer = layer;
					else
						ENGINE_ERROR("[Entity::setLayer] New layer doesn't exist in the entity's parent scene. Entity: {0}.", m_entityName);
				}
				else
					ENGINE_ERROR("[Entity::setLayer] The parent scene does not have a valid layer manager. Scene: {0}, Entity: {1}.", scene->getName(), m_entityName);
			}
			else
				ENGINE_ERROR("[Entity::setLayer] The entity does not have a valid parent scene. Entity: {0}.", m_entityName);
		}
		else
			ENGINE_ERROR("[Entity::setLayer] New layer is a null pointer. Entity: {0}.", m_entityName);
	}

	//! getLayer()
	/*!
	\return a Layer* - A pointer to the layer
	*/
	Layer* Entity::getLayer() const
	{
		if (!m_layer)
			ENGINE_ERROR("[Entity::getLayer] The Layer is a null pointer.");
		return m_layer;
	}

	//! setDisplay()
	/*!
	\param display a const bool - Should the entity be displayed
	*/
	void Entity::setDisplay(const bool display)
	{
		m_display = display;
	}

	//! getDisplay()
	/*!
	\return a const bool - Whether the entity should be displayed
	*/
	const bool Entity::getDisplay() const
	{
		return m_display;
	}

	//! containsPoint()
	/*!
	\param coordinate a const glm::vec2& - The coordinate to check
	\return a bool - Is this coordinate within this bounding box
	*/
	bool Entity::containsPoint(const glm::vec2& coordinate)
	{
		// Need to convert bottom left origin to calc top left and right, manually input scale and tune values size isnt known
		auto transform = getComponent<Transform>();

		if (transform)
		{
			glm::vec2 pos = { transform->getWorldPosition().x, transform->getWorldPosition().y };
			glm::vec2 topLeft = pos - glm::vec2(0.f, transform->getScale().y);
			glm::vec2 bottomRight = pos + glm::vec2(transform->getScale().x, 0.f);

			if (coordinate.x >= topLeft.x && coordinate.x <= bottomRight.x)
				if (coordinate.y >= topLeft.y && coordinate.y <= bottomRight.y)
					return true;
		}
		else
			ENGINE_ERROR("[Entity::containsPoint] This entity does not have a valid transform. Entity Name: {0}.", m_entityName);

		return false;
	}

	//! addChildEntity()
	/*
	\param childName a const std::string& - The name of the child
	\param entity an Entity* - A pointer to the entity
	\return a bool - The success of adding the child entity
	*/
	bool Entity::addChildEntity(const std::string& childName, Entity* entity)
	{
		// Add entity
		if (!checkChildEntityNameTaken(childName) && childName != "")
		{
			// If an invalid entity was provided, just exit out the function
			if (!entity)
			{
				ENGINE_ERROR("[Entity::addChildEntity] An invalid entity pointer was provided for child entity name: {0} for entity: {1}.", childName, m_entityName);
				return false;
			}

			if (entity->getParentEntity() != nullptr)
				entity->getParentEntity()->getChildEntities().erase(entity->getName());

			// Set the parent scene of the entity and its name
			m_childEntities[childName] = entity;
			entity->setParentScene(getParentScene());
			entity->setParentEntity(this);
			entity->setName(childName);
			getParentScene()->setEntityListUpdated(true);
			return true;
		}
		else
			ENGINE_ERROR("[Entity::addChildEntity] Name for child entity already taken. Cannot add. Child entity Name: {0} for entity: {1}.", childName, m_entityName);
		return false;
	}

	//! checkChildEntityNameTaken()
	/*!
	\param name a const std::string& - The name of the entity
	\return a bool - Does the entity name exist in our entity list
	*/
	bool Entity::checkChildEntityNameTaken(const std::string& name) const
	{
		// Check if the name given exists in the entity list
		if (m_childEntities.find(name) != m_childEntities.end() && name != "")
			return true;
		return false;
	}

	void Entity::setSelected(bool select)
	{
		m_selected = select;
	}

	bool Entity::getSelected()
	{
		return m_selected;
	}

	//! getAllComponents()
	/*!
	\return a std::vector<EntityComponent*>& - A reference to the list of all components
	*/
	std::vector<EntityComponent*>& Entity::getAllComponents()
	{
		return m_components;
	}

	//! detach()
	/*
	\param component an EntityComponent* - A pointer to the component
	*/
	void Entity::detach(EntityComponent* component)
	{
		if (component)
		{
			// If it does, detach and delete
			component->onDetach();
			delete component;
			m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
		}
		else
			ENGINE_ERROR("[Entity::detach] The entity does not have a component of this type. Entity Name: {0}, Component Type: {1}.", m_entityName, Components::toString(component->getComponentType()));
	}

	//! detachDelayDeletion()
	/*
	\param component an EntityComponent* - A pointer to the component
	*/
	void Entity::detachDelayDeletion(EntityComponent* component)
	{
		if (component)
		{
			// If it does, detach and delete
			component->onDetach();
			m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
		}
		else
			ENGINE_ERROR("[Entity::detachDelayDeletion] The entity does not have a component of this type. Entity Name: {0}, Component Type: {1}.", m_entityName, Components::toString(component->getComponentType()));
	}

	//! printEntityDetails()
	void Entity::printEntityDetails()
	{
		Scene* scene = getParentScene();

		if (!scene)
		{
			ENGINE_ERROR("[Entity::printEntityDetails] This entity does not have a valid parent scene. Entity Name: {0}.", m_entityName);
			return;
		}

		ENGINE_TRACE("==============");
		ENGINE_TRACE("Entity Details");
		ENGINE_TRACE("==============");
		ENGINE_TRACE("Entity Address: {0}.", (void*)this);
		ENGINE_TRACE("Entity Name: {0}.", getName());
		ENGINE_TRACE("Entity Parent Scene: {0} [Name: {1}].", (void*)scene, scene->getName());
		ENGINE_TRACE("Attached to Layer: {0}.", (void*)getLayer());
		ENGINE_TRACE("Entity Component Count: {0}", getAllComponents().size());
		for (auto& comp : getAllComponents())
		{
			if(comp)
				ENGINE_TRACE("Component Type: {0}.", static_cast<int>(comp->getComponentType()));
		}
		ENGINE_TRACE("Scheduled for Deletion: {0}.", getDestroyed());
		ENGINE_TRACE("==============");
	}
}
