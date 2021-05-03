/*! \file engineScript.h
*
* \brief A script to use some engine features
*
* \author: Daniel Bullin
*
*/
#ifndef ENGINESCRIPT_H
#define ENGINESCRIPT_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class EngineScript
* \brief A script to use some engine features
*/
class EngineScript : public NativeScript
{
public:
	EngineScript(); //!< Constructor
	~EngineScript(); //!< Destructor
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif