/*! \file cyborg.cpp
*
* \brief A Cyborg dummy class
*
* \author: Daniel Bullin
*
*/
#include "entities/cyborg.h"
#include "independent/systems/systems/resourceManager.h"

//! Cyborg()
Cyborg::Cyborg()
{
}

//! ~Cyborg()
Cyborg::~Cyborg()
{
}

//! onPreUpdate()
/*
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onPreUpdate(const float timestep, const float totalTime)
{
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::P)
	{
		setDisplay(!getDisplay());
	}
}
