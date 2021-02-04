/*! \file layerManager.h
*
* \brief A layer manager which manages layers
*
* \author Daniel Bullin
*
*/
#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "independent/core/common.h"
#include "independent/layers/layer.h"

namespace Engine 
{
	class Scene; //!< Forward declare scene

	/*! \class LayerManager
	* \brief A layer manager which manages layers
	*/
	class LayerManager {
	private:
		Scene* m_attachedScene; //!< The scene this manager is attached to
		std::vector<Layer*> m_layers; //!< A list of all layers
	public:
		LayerManager(Scene* parentScene); //!< Constructor
		~LayerManager(); //!< Destructor

		void attachLayer(Layer* newLayer); //!< Add a layer to the layer list
		void destroy(); //!< Destroys the layer manager and all layers
		std::vector<Layer*>& getLayers(); //!< Get all layers
		Layer* getLayer(const std::string& layerName); //!< Get a layer by name

		Scene* getParentScene(); //!< Get the scene this manager is attached to

		void onUpdate(const float timestep, const float totalTime); //!< Called once every frame

		void printLayerManagerDetails(); //!< Print the layer manager details
		void printLayerDetails(); //!< Print the layer details
	};
}
#endif