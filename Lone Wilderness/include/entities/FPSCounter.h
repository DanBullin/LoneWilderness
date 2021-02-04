/*! \file FPSCounter.h
*
* \brief A FPS counter text dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "independent/entities/entity.h"

using namespace Engine;

/*! \class FPSCounter
* \brief A FPSCounter dummy class
*/
class FPSCounter : public Entity
{
private:

public:
	FPSCounter(); //!< Constructor
	~FPSCounter(); //!< Destructor
	virtual void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	virtual void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
};
#endif