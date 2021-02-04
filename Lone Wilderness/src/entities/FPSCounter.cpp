/*! \file FPSCounter.cpp
*
* \brief A FPS counter text dummy class
*
* \author: Daniel Bullin
*
*/
#include "entities/FPSCounter.h"
#include "independent/systems/systemManager.h"

//! FPSCounter()
FPSCounter::FPSCounter()
{
	TimerSystem::startTimer("FPSRefresh");
}

//! ~FPSCounter()
FPSCounter::~FPSCounter()
{
	
}

void FPSCounter::onPostUpdate(const float timestep, const float totalTime)
{
	if (TimerSystem::getTime("FPSRefresh", false, true) > 1.f)
	{
		getComponent<Text>()->setText("FPS: " + std::to_string(static_cast<uint32_t>(TimerSystem::getFPS())));
		TimerSystem::stopTimer("FPSRefresh", false, true);
	}
}

void FPSCounter::onPreUpdate(const float timestep, const float totalTime)
{
	
}
