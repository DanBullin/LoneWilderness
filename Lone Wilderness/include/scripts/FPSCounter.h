/*! \file FPSCounter.h
*
* \brief A FPS counter text dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class FPSCounter
* \brief A FPSCounter script
*/
class FPSCounter : public NativeScript
{
public:
	FPSCounter(); //!< Constructor
	~FPSCounter(); //!< Destructor
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
};
#endif