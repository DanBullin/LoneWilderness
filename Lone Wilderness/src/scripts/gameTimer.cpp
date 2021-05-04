/*! \file gameTimer.cpp
*
* \brief A game timer script
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameTimer.h"
#include "independent/systems/systems/log.h"

GameTimer::GameTimer()
{
	m_time = 0;
	m_enabled = true;
}

GameTimer::~GameTimer()
{
}

void GameTimer::onPreUpdate(const float timestep, const float totalTime)
{
	if (m_enabled)
	{
		m_time += timestep;
	}
}

void GameTimer::start()
{
	m_enabled = true;
}

void GameTimer::stop()
{
	m_enabled = false;
}

void GameTimer::reset()
{
	m_time = 0;
}

bool GameTimer::isEnabled()
{
	return m_enabled;
}

const float GameTimer::getTime()
{
	return m_time;
}