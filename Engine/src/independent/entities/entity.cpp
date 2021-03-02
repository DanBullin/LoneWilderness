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
		m_layer = nullptr;
		m_display = true;
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
			glm::vec2 pos = { transform->getPosition().x, transform->getPosition().y };
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

	//! getAllComponents()
	/*!
	\return a const std::vector<EntityComponent*>& - A reference to the list of all components
	*/
	const std::vector<EntityComponent*>& Entity::getAllComponents()
	{
		return m_components;
	}

	//! detach()
	/*
	\param type a const ComponentType - The type of component to delete
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

	//! onRender()
	/*!
	\param renderer a const Renderers - The renderer type
	*/
	void Entity::onRender(const Renderers renderer)
	{
		// This function will be called if it's not overridden by child entity classes
		// This provides this default action which can be overriden if sandbox needs more control over what and when to render an entity
		if (renderer == Renderers::Renderer3D)
		{
			if (containsComponent<MeshRender3D>())
			{
				MeshRender3D* meshRender = getComponent<MeshRender3D>();
				if (meshRender)
				{
					Material* material = meshRender->getMaterial();

					if (!material || material->getName() == "")
						material = ResourceManager::getResource<Material>("defaultMaterial3D");

					for (auto& mesh : meshRender->getModel()->getMeshes())
						Renderer3D::submit(mesh.getGeometry(), material, getComponent<Transform>()->getModelMatrix());
				}
			}
		}
		else if (renderer == Renderers::Renderer2D)
		{
			if (containsComponent<MeshRender2D>())
			{
				MeshRender2D* meshRender = getComponent<MeshRender2D>();
				Renderer2D::submit(meshRender->getMaterial()->getShader(), meshRender->getMaterial()->getSubTextures(), getComponent<Transform>()->getModelMatrix(), meshRender->getMaterial()->getTint());
			}
			if (containsComponent<Text>())
			{
				// Submit all text components
				Text* textRender = getComponent<Text>();
				Renderer2D::submitText(textRender, getComponent<Transform>()->getModelMatrix());
			}
		}
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
