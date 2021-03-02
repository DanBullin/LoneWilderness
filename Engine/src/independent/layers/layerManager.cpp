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
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//! LayerManager()
	/*!
	\param scene a Scene* - A pointer to the scene
	*/
	LayerManager::LayerManager(Scene* scene)
	{
		m_layers.reserve(ResourceManager::getConfigValue(Config::MaxLayersPerScene));

		if (scene)
			m_attachedScene = scene;
		else
			ENGINE_ERROR("[LayerManager::LayerManager] An invalid scene was provided.");
	}

	//! ~LayerManager()
	LayerManager::~LayerManager()
	{
		if (getParentScene())
		{
			// Delete all layers
			ENGINE_INFO("[LayerManager::~LayerManager] Deleting layer manager attached to {0}.", getParentScene()->getName());
			m_attachedScene = nullptr;
		}
		else
			ENGINE_INFO("[LayerManager::~LayerManager] Deleting layer manager which has no scene attached.");

		// Go through the list in reverse
		for (std::vector<Layer*>::reverse_iterator i = m_layers.rbegin(); i != m_layers.rend(); ++i)
		{
			// Stop the layer
			if ((*i))
			{
				// Call the onDetach function
				(*i)->onDetach();
				if ((*i)) delete (*i);
			}
			else
				ENGINE_ERROR("[LayerManager::~LayerManager] Layer is a null pointer.");
		}
		// All layers have been detached, clear the list
		m_layers.clear();
	}

	//! attachLayer()
	/*!
	\param newLayer a Layer* - A pointer to the layer
	*/
	void LayerManager::attachLayer(Layer* newLayer)
	{
		if (!newLayer)
		{
			ENGINE_ERROR("[LayerManager::attachLayer] An invalid layer was provided.");
			return;
		}

		// Loop through each layer and check its name, if it exists, just exit out of function
		for (auto& layer : m_layers)
		{
			if (layer)
			{
				if (layer->getLayerName() == newLayer->getLayerName())
				{
					ENGINE_ERROR("[LayerManager::attachLayer] Layer name is already taken. Name: {0}.", layer->getLayerName());
					return;
				}
			}
		}

		// Add layer to the list
		m_layers.push_back(newLayer);
		// Update the layer's manager
		newLayer->setLayerManager(this);
		// Call the layer's onAttach function
		newLayer->onAttach();
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
			if (layer)
			{
				if (layer->getLayerName() == layerName)
					return layer;
			}
		}
		ENGINE_ERROR("[LayerManager::getLayer] Could not get layer as layer name could not be found. Name: {0}, Scene: {1}.", layerName, getParentScene()->getName());
		return nullptr;
	}

	//! getLayers()
	/*!
	\return a std::vector<Layer*>& - A reference to the list of layers
	*/
	std::vector<Layer*>& LayerManager::getLayers()
	{
		return m_layers;
	}

	//! layerExists()
	/*!
	\param layer a Layer* - A pointer to the layer
	\return a bool - Does the layer exist in the layer list
	*/
	bool LayerManager::layerExists(Layer* layer)
	{
		// Loop through each layer and check its name
		for (auto& layerElement : m_layers)
		{
			if (layer)
			{
				if (layerElement->getLayerName() == layer->getLayerName())
					return true;
			}
			else
				ENGINE_ERROR("[LayerManager::layerExists] An invalid layer was provided.");
		}
		return false;
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
			if (layer)
			{
				if (layer->getActive())
					layer->onUpdate(timestep, totalTime);
			}
		}
	}

	//! printLayerManagerDetails()
	void LayerManager::printLayerManagerDetails()
	{
		ENGINE_TRACE("Layer Manager Details");
		ENGINE_TRACE("============================================");
		ENGINE_TRACE("Layer List Size: {0}", m_layers.size());
		if(m_attachedScene)
			ENGINE_TRACE("Attached to scene: {0}", getParentScene()->getName());
		else
			ENGINE_TRACE("Attached to scene: NULL");
		ENGINE_TRACE("============================================");
	}

	//! printLayerDetails()
	void LayerManager::printLayerDetails()
	{
		for (auto& layer : m_layers)
		{
			if(layer) layer->printLayerDetails();
		}
	}
}