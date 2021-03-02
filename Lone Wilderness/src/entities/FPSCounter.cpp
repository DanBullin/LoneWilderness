/*! \file FPSCounter.cpp
*
* \brief A FPS counter text dummy class
*
* \author: Daniel Bullin
*
*/
#include "entities/FPSCounter.h"
#include "independent/systems/systems/timerSystem.h"

//! FPSCounter()
FPSCounter::FPSCounter()
{
	TimerSystem::startTimer("FPSRefresh");
}

//! ~FPSCounter()
FPSCounter::~FPSCounter()
{
}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void FPSCounter::onPreUpdate(const float timestep, const float totalTime)
{
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void FPSCounter::onPostUpdate(const float timestep, const float totalTime)
{
	if (TimerSystem::getTime("FPSRefresh", false, true) > 1.f)
	{
		Text* text = getComponent<Text>();
		
		if (text)
			text->setText("FPS: " + std::to_string(static_cast<uint32_t>(TimerSystem::getFPS())));
		
		TimerSystem::stopTimer("FPSRefresh", false, true);
	}
}