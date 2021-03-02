/*! \file sign.cpp
*
* \brief A Neon Sign environmental entity
*
* \author: Jamie Hall
*
*/
#include "entities/sign.h"

//! Sign()
Sign::Sign()
{
}

//! ~Sign()
Sign::~Sign()
{
}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Sign::onPreUpdate(const float timestep, const float totalTime)
{
	
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Sign::onPostUpdate(const float timestep, const float totalTime)
{

}
void Sign::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::K)
		setDisplay(!getDisplay());
}