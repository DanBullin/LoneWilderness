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
			delete comp.second;
		
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
	\param entityName a const char* - The name of the entity
	*/
	void Entity::setName(const char* entityName)
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
			// Check if new layer exists in the entity's parent scene layers
			if(getParentScene()->getLayerManager()->layerExists(layer))
				m_layer = layer;
			else
				ENGINE_ERROR("[Entity::setLayer] New layer doesn't exist in the entity's parent scene. Entity: {0}.", m_entityName);
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

	//! detach()
	/*!
	\param name a const char* - The name of the component
	*/
	void Entity::detach(const char* name)
	{
		if (componentNameExists(name))
		{
			// Check if the component is already deleted
			if (m_components[name])
			{
				// Call the component's onDetach function
				m_components[name]->onDetach();
				// Found component, clean up
				delete m_components[name];
				m_components.erase(name);
			}
			else
				ENGINE_ERROR("[Entity::detach] The component is a null pointer. Entity Name: {0}, Component: {1}", m_entityName, name);
		}
		else
			ENGINE_ERROR("[Entity::detach] Cannot find component. Entity Name: {0}, Component: {1}", m_entityName, name);
	}

	//! getAllComponents()
	/*!
	\return a std::map<std::string, EntityComponent*>& - A reference to the list of all components
	*/
	std::map<std::string, EntityComponent*>& Entity::getAllComponents()
	{
		return m_components;
	}

	//! componentNameExists()
	/*!
	\param name a const std::string& - The name of the component
	\return a const bool - Was the component name found in the component list
	*/
	bool Entity::componentNameExists(const std::string& name)
	{
		return m_components.find(name) != m_components.end();
	}

	//! onRender()
	/*!
	\param renderer a const Renderers - The renderer type
	*/
	void Entity::onRender(const Renderers renderer)
	{
		// This function will be called if it's not overriden by child entity classes
		// This provides this default action which can be overriden if sandbox needs more control
		// over what and when to render an entity
		if (renderer == Renderers::Renderer3D)
		{
			if (containsComponent<MeshRender3D>())
			{
				// Submit all 3D mesh renders
				for (auto& meshRender : getComponents<MeshRender3D>())
				{
					for (auto& mesh : meshRender->getModel()->getMeshes())
						Renderer3D::submit(mesh.getGeometry(), meshRender->getMaterial(), meshRender->getModelMatrix());
				}
			}
		}
		else if (renderer == Renderers::Renderer2D)
		{
			if (containsComponent<MeshRender2D>())
			{
				// Submit all 2D mesh renders
				for (auto& meshRender : getComponents<MeshRender2D>())
					Renderer2D::submit(meshRender->getMaterial(), meshRender->getModelMatrix());
			}
			if (containsComponent<Text>())
			{
				// Submit all text components
				for (auto& textRender : getComponents<Text>())
					Renderer2D::submitText(textRender, textRender->getModelMatrix());
			}
		}
	}

	//! printEntityDetails()
	void Entity::printEntityDetails()
	{
		ENGINE_TRACE("==============");
		ENGINE_TRACE("Entity Details");
		ENGINE_TRACE("==============");
		ENGINE_TRACE("Entity Address: {0}.", (void*)this);
		ENGINE_TRACE("Entity Name: {0}.", getName());
		ENGINE_TRACE("Entity Parent Scene: {0} [Name: {1}].", (void*)getParentScene(), getParentScene()->getName());
		ENGINE_TRACE("Attached to Layer: {0}.", (void*)getLayer());
		ENGINE_TRACE("Entity Component Count: {0}", getAllComponents().size());
		for (auto& comp : getAllComponents())
		{
			ENGINE_TRACE("Component Name: {0}, Type: {1}.", comp.second->getName(), static_cast<int>(comp.second->getComponentType()));
		}
		ENGINE_TRACE("Scheduled for Deletion: {0}.", getDestroyed());
		ENGINE_TRACE("==============");
	}

	//! convertComponentClassType()
	/*!
	\param classType a const std::string& - The class type (Use typeid)
	\return a const ComponentType - The type of component
	*/
	const ComponentType Entity::convertComponentClassType(const std::string& classType)
	{
		// Class types should be in the format [class Engine::{CLASS_NAME}]
		if (classType == "class Engine::Camera" || classType == "Camera")
			return ComponentType::Camera;
		else if (classType == "class Engine::Transform3D" || classType == "Transform3D")
			return ComponentType::Transform3D;
		else if (classType == "class Engine::Transform2D" || classType == "Transform2D")
			return ComponentType::Transform2D;
		else if (classType == "class Engine::EventListener" || classType == "EventListener")
			return ComponentType::EventListener;
		else if (classType == "class Engine::MeshRender3D" || classType == "MeshRender3D")
			return ComponentType::MeshRender3D;
		else if (classType == "class Engine::MeshRender2D" || classType == "MeshRender2D")
			return ComponentType::MeshRender2D;
		else if (classType == "class Engine::Text" || classType == "Text")
			return ComponentType::Text;
		else
			return ComponentType::None;
	}
}
