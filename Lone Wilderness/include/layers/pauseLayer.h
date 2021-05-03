/*! \file pauseLayer.h
*
* \brief The pause layer containing all entities to do with the pause menu
*
* \author: Daniel Bullin
*
*/
#ifndef PAUSELAYER_H
#define PAUSELAYER_H

#include "independent/layers/layer.h"

using namespace Engine;

/*! \class PauseLayer
* \brief A layer for the pause menu entities
*/
class PauseLayer : public Layer
{
public:
	PauseLayer(); //!< Constructor
	~PauseLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
};
#endif