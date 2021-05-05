/*! \file craftingLayer.h
*
* \brief The crafting layer containing all entities to do with the crafting menu
*
* \author: Daniel Bullin
*
*/
#ifndef CRAFTINGLAYER_H
#define CRAFTINGLAYER_H

#include "independent/layers/layer.h"

using namespace Engine;

/*! \class CraftingLayer
* \brief A layer for the crafting menu entities
*/
class CraftingLayer : public Layer
{
public:
	CraftingLayer(); //!< Constructor
	~CraftingLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
};
#endif