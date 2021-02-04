/*! \file timer.cpp
*
* \brief A class to measure time
*
* \author DMU Course material
*
*/

#include "independent/systems/components/timer.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! ~Timer()
	Timer::~Timer()
	{

	}

	//! ChronoTimer()
	/*!
	\param a const char* timerName - The name of the timer
	*/
	ChronoTimer::ChronoTimer(const char* timerName) : m_timerName(timerName)
	{
		ENGINE_INFO("[ChronoTimer::ChronoTimer] Creating a timer named: {0}.", m_timerName);
	}

	//! ~ChronoTimer()
	ChronoTimer::~ChronoTimer()
	{
		ENGINE_INFO("[ChronoTimer::~ChronoTimer] Destroying a timer named: {0}.", m_timerName);
	}

	//! getElapsedTime()
	/*!
	\return a const float - The time in seconds
	*/
	const float ChronoTimer::getElapsedTime()
	{
		// Measure the time based on the end point and the start
		m_endPoint = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> elapsed = m_endPoint - m_startPoint;
		return elapsed.count() / 1000.f;
	}
}