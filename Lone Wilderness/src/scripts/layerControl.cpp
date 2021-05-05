/*! \file layerControl.cpp
*
* \brief A script to help control layers more easily
*
* \author: Daniel Bullin
*
*/
#include "scripts/layerControl.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"

//! LayerControl()
LayerControl::LayerControl()
{
}

//! ~LayerControl()
LayerControl::~LayerControl()
{
}

//! hide()
/*!
\param layerName a const std::string - The name of the layer to hide
*/
void LayerControl::hide(const std::string& layerName, Scene* scene)
{
	auto layer = scene->getLayerManager()->getLayer(layerName);
	if (layer)
	{
		layer->setDisplayed(false);
	}
}

//! show()
/*!
\param layerName a const std::string - The name of the layer to show
*/
void LayerControl::show(const std::string& layerName, Scene* scene)
{
	auto layer = scene->getLayerManager()->getLayer(layerName);
	if (layer)
	{
		layer->setDisplayed(true);
	}
}

//! activate()
/*!
\param layerName a const std::string - The name of the layer to activate
*/
void LayerControl::activate(const std::string & layerName, Scene* scene)
{
	auto layer = scene->getLayerManager()->getLayer(layerName);
	if (layer)
	{
		layer->setActive(true);
	}
}

//! deactivate()
/*!
\param layerName a const std::string - The name of the layer to deactivate
*/
void LayerControl::deactivate(const std::string & layerName, Scene* scene)
{
	auto layer = scene->getLayerManager()->getLayer(layerName);
	if (layer)
	{
		layer->setActive(false);
	}
}

bool LayerControl::isShow(const std::string& layerName, Scene* scene)
{
	auto layer = scene->getLayerManager()->getLayer(layerName);
	if (layer)
	{
		return layer->getDisplayed();
	}
	return false;
}
