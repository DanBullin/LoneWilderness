/*! \file layer.cpp
*
* \brief A layer containing a group of scene data
*
* \author DMU Course material
*
*/

#include "independent/layers/layer.h"
#include "independent/layers/layerManager.h"
#include "independent/systems/components/scene.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! Layer()
	/*!
	\param layerName a const char* - The layer name
	*/
	Layer::Layer(const char* layerName) : m_layerName(layerName), m_display(true), m_active(true), m_layerManager(nullptr)
	{
	}

	//! ~Layer()
	Layer::~Layer()
	{
		if (getLayerManager())
		{
			if(getLayerManager()->getParentScene())
				ENGINE_INFO("[Layer::~Layer] Deleting layer named: {0} which is attached to scene: {1}.", m_layerName, getLayerManager()->getParentScene()->getName());
		}
		else
			ENGINE_INFO("[Layer::~Layer] Deleting layer named: {0}.", m_layerName);
	}

	//! printLayerDetails()
	void Layer::printLayerDetails()
	{
		ENGINE_TRACE("Layer Details for {0}", getLayerName());
		ENGINE_TRACE("===================================");
		ENGINE_TRACE("Display: {0}.", getDisplayed());
		ENGINE_TRACE("Active: {0}.", getActive());
		ENGINE_TRACE("Layer Manager: {0}.", (void*)m_layerManager);
		ENGINE_TRACE("Number of entities on layer: {0}.", getEntities().size());
		ENGINE_TRACE("===================================");
	}

	//! getLayerName()
	/*!
	\return a const std::string& - The layer name
	*/
	const std::string& Layer::getLayerName()
	{
		return m_layerName;
	}

	//! getDisplayed()
	/*!
	\return a const bool - Is this layer displayed
	*/
	const bool Layer::getDisplayed() const
	{
		return m_display;
	}

	//! setDisplayed()
	/*!
	\param display a const bool - Is this layer displayed
	*/
	void Layer::setDisplayed(const bool display)
	{
		m_display = display;
	}

	//! getActive()
	/*!
	\return a const bool - Is this layer active
	*/
	const bool Layer::getActive() const
	{
		return m_active;
	}

	//! setActive()
	/*!
	\param active a const bool - Is this layer active
	*/
	void Layer::setActive(const bool active)
	{
		m_active = active;
	}

	//! setLayerManager()
	/*!
	\param manager a LayerManager* - The layer manager this layer is attached to
	*/
	void Layer::setLayerManager(LayerManager* manager)
	{
		if (manager)
			m_layerManager = manager;
		else
			ENGINE_INFO("[Layer::setLayerManager] The manager given is a null pointer.");
	}

	//! getLayerManager()
	/*!
	\return a LayerManager* - The layer manager this layer is attached to
	*/
	LayerManager* Layer::getLayerManager() const
	{
		return m_layerManager;
	}

	//! getEntities()
	/*!
	\return a std::vector<Entity*> - A list of all entities on this layer
	*/
	std::vector<Entity*> Layer::getEntities()
	{
		std::vector<Entity*> list;

		// Go through all the entities
		for (auto& entity : getLayerManager()->getParentScene()->getRootEntities())
		{
			// If the layer the entity is on matches this layer
			if (entity.second->getLayer() == this)
				list.emplace_back(entity.second);
		}
		return list;
	}

	//! getRenderableEntities()
	/*!
	\return a std::vector<Entity*> - A list of all renderable entities on this layer
	*/
	std::vector<Entity*> Layer::getRenderableEntities()
	{
		std::vector<Entity*> list;

		// Go through all the entities
		for (auto& entity : getLayerManager()->getParentScene()->getRootEntities())
		{
			// If the layer the entity is on matches this layer
			if (entity.second->getLayer() == this)
			{
				// If the entity contains some rendering component
				if(entity.second->containsComponent<MeshRender3D>() || entity.second->containsComponent<MeshRender2D>() || entity.second->containsComponent<Text>())
					list.emplace_back(entity.second);
			}
		}
		return list;
	}
}