/*! \file inventoryLayer.h
*
* \brief The inventory layer containing all entities to do with the inventory menu
*
* \author: Daniel Bullin
*
*/
#ifndef INVENTORYLAYER_H
#define INVENTORYLAYER_H

#include "independent/layers/layer.h"

using namespace Engine;

/*! \class InventoryLayer
* \brief A layer for the inventory menu entities
*/
class InventoryLayer : public Layer
{
public:
	InventoryLayer(); //!< Constructor
	~InventoryLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
};
#endif