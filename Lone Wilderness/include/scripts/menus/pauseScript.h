/*! \file pauseScript.h
*
* \brief A script to show the pause menu
*
* \author: Daniel Bullin
*
*/
#ifndef PAUSESCRIPT_H
#define PAUSESCRIPT_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class PauseScript
* \brief A script which shows the pause menu
*/
class PauseScript : public NativeScript
{
public:
	PauseScript(); //!< Constructor
	~PauseScript(); //!< Destructor
	void onAttach() override; //!< On attach
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif