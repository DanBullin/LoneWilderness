/*! \file layerManager.cpp
*
* \brief A layer manager which manages layers
*
* \author Daniel Bullin
*
*/

#include "independent/layers/layerManager.h"
#include "independent/systems/components/scene.h"
#include "independent/systems/systems/log.h"

#define LAYERCAPACITY 25

namespace Engine
{
	//! LayerManager()
	/*!
	\param scene a Scene* - A pointer to the scene
	*/
	LayerManager::LayerManager(Scene* scene)
	{
		m_layers.reserve(LAYERCAPACITY);
		m_attachedScene = scene;
	}

	//! ~LayerManager()
	LayerManager::~LayerManager()
	{
		// Delete all layers
		ENGINE_INFO("[LayerManager::~LayerManager] Deleting layer manager attached to {0}.", getParentScene()->getName());
		destroy();
		m_attachedScene = nullptr;
	}

	//! attachLayer()
	/*!
	\param newLayer a Layer* - A pointer to the layer
	*/
	void LayerManager::attachLayer(Layer* newLayer)
	{
		// Loop through each layer and check its name, if it exists, just exit out of function
		for (auto& layer : m_layers)
		{
			if (layer->getLayerName() == newLayer->getLayerName())
			{
				ENGINE_ERROR("[LayerManager::attachLayer] Layer name is already taken. Name: {0}. Scene Name: {1}.", layer->getLayerName(), getParentScene()->getName());
				delete newLayer;
				return;
			}
		}

		m_layers.push_back(newLayer);
		newLayer->setLayerManager(this);
		newLayer->onAttach();
	}

	//! destroy()
	void LayerManager::destroy()
	{
		// Go through the list in reverse
		for (std::vector<Layer*>::reverse_iterator i = m_layers.rbegin(); i != m_layers.rend(); ++i)
		{
			// Stop the layer
			if ((*i))
			{
				(*i)->onDetach();
				delete (*i);
			}
			else
				ENGINE_ERROR("[LayerManager::destroy] Layer is a null pointer.");
		}
		// All layers have been detached, clear the list
		m_layers.clear();
	}

	//! getLayers()
	/*!
	\return a std::vector<Layer*>& - A reference to the list of layers
	*/
	std::vector<Layer*>& LayerManager::getLayers()
	{
		return m_layers;
	}

	//! getLayer()
	/*!
	\param layerName a const std::string& - The name of the layer
	\return a Layer* - A pointer to the layer
	*/
	Layer* LayerManager::getLayer(const std::string& layerName)
	{
		// Loop through each layer and check its name
		for (auto& layer : m_layers)
		{
			if (layer->getLayerName() == layerName)
				return layer;
		}
		ENGINE_ERROR("[LayerManager::getLayer] Could not get layer as layer name could not be found. Name: {0}, Scene: {1}.", layerName, getParentScene()->getName());
		return nullptr;
	}

	//! getParentScene()
	/*!
	\return a Scene* - The parent scene
	*/
	Scene* LayerManager::getParentScene()
	{
		return m_attachedScene;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void LayerManager::onUpdate(const float timestep, const float totalTime)
	{
		// Update all layers
		for (auto& layer : m_layers)
		{
			if (layer->getActive())
				layer->onUpdate(timestep, totalTime);
		}
	}

	//! printLayerManagerDetails()
	void LayerManager::printLayerManagerDetails()
	{
		ENGINE_TRACE("Layer Manager Details for Scene: {0}", getParentScene()->getName());
		ENGINE_TRACE("============================================");
		ENGINE_TRACE("Layer List Size: {0}", m_layers.size());
		ENGINE_TRACE("============================================");
	}

	//! printLayerDetails()
	void LayerManager::printLayerDetails()
	{
		for (auto& layer : m_layers)
			layer->printLayerDetails();
	}
}