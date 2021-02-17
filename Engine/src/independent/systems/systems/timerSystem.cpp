/*! \file timerSystem.cpp
*
* \brief A timer system which manages timers and stores recorded times
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/timerSystem.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	bool TimerSystem::s_enabled = false; //!< Is this system enabled
	std::map<const char*, std::pair<Timer*, float>> TimerSystem::s_timers = std::map<const char*, std::pair<Timer*, float>>(); //!< All running timers

	//! TimerSystem
	TimerSystem::TimerSystem() : System(SystemType::TimerSystem)
	{
	}

	//! ~TimerSystem
	TimerSystem::~TimerSystem()
	{
	}

	//! start()
	void TimerSystem::start()
	{
		// Start system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[TimerSystem::start] Starting the timer system.");
			s_enabled = true;
		}
	}

	//! stop()
	void TimerSystem::stop()
	{
		// Stop system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[TimerSystem::stop] Stopping the timer system.");

			// Remove all timers if the list is not empty
			removeTimers();
			s_enabled = false;
		}
	}

	//! startTimer()
	/*!
	\param timerName a const char* - The timer name
	*/
	void TimerSystem::startTimer(const char* timerName)
	{
		if (s_enabled)
		{
			// Reset the timer if it exists otherwise create a new timer
			if (timerExistsByName(timerName))
				s_timers[timerName].first->reset();
			else
			{
				// Create new timer and it's associated result
				s_timers[timerName] = std::make_pair(new ChronoTimer(timerName), 0.f);
				s_timers[timerName].first->start();
			}
		}
		else
			ENGINE_ERROR("[TimerSystem::startTimer] This system has not been enabled.");
	}

	//! stopTimer()
	/*!
	\param timerName a const char* - The timer name
	\param deleteTimer a const bool - Delete the timer
	\param storeResult a const bool - Store the result
	*/
	void TimerSystem::stopTimer(const char* timerName, const bool deleteTimer, const bool storeResult)
	{
		if (s_enabled)
		{
			// Reset the timer or delete it
			// First check if timer exists
			if (timerExistsByName(timerName))
			{
				// If timer is to be deleted, remove timer
				if (deleteTimer)
					removeTimer(timerName);
				else
				{
					// Timer will not be deleted, do we need to store the current time
					if (storeResult)
						s_timers[timerName].second = s_timers[timerName].first->getElapsedTime();

					s_timers[timerName].first->reset();
				}
			}
		}
		else
			ENGINE_ERROR("[TimerSystem::endTimer] This system has not been enabled.");
	}

	//! getTime()
	/*!
	\param timerName a const char* - The timer name
	\param reset a const bool - Reset the timer
	\param storeResult a const bool - Store the result
	\return a const float - The recorded time
	*/
	const float TimerSystem::getTime(const char* timerName, const bool reset, const bool storeResult)
	{
		// Returns the current time of the timer and ability to reset the time
		float result = 0.f;

		if (s_enabled)
		{
			// Check if timer exists
			if (timerExistsByName(timerName))
			{
				// Get the elapsed time
				result = s_timers[timerName].first->getElapsedTime();

				// Reset timer if we want to reset
				if (reset)
					s_timers[timerName].first->reset();

				// Check if we're to store the result
				if (storeResult)
					s_timers[timerName].second = result;
			}
		}
		else
			ENGINE_ERROR("[TimerSystem::getTime] This system has not been enabled.");

		return result;
	}

	//! getStoredTime()
	/*!
	\param timerName a const char* - The timer name
	\return a const float - The recorded time
	*/
	const float TimerSystem::getStoredTime(const char* timerName)
	{
		// Check if timer exists
		if (s_enabled)
		{
			if (timerExistsByName(timerName))
				return s_timers[timerName].second;
		}
		else
			ENGINE_ERROR("[TimerSystem::getStoredTime] This system has not been enabled.");

		// Timer doesn't exist, return 0
		return 0.f;
	}

	//! getTimePerSecond()
	/*!
	\param timerName a const char* - The timer name
	\param reset a const bool - Reset the time?
	\param storeResult a const bool - Store the result
	\return a const float - The recorded time
	*/
	const float TimerSystem::getTimePerSecond(const char* timerName, const bool reset, const bool storeResult)
	{
		float result = 0.f;
		
		if (s_enabled)
		{
			// Check if timer exists
			if (timerExistsByName(timerName))
			{
				// Divide result from 1
				result = static_cast<float>(1.f / s_timers[timerName].first->getElapsedTime());

				// Reset time if we're told to
				if (reset)
					s_timers[timerName].first->reset();

				// Check if we're to store the result
				if (storeResult)
					s_timers[timerName].second = result;
			}
		}
		else
			ENGINE_ERROR("[TimerSystem::getTimePerSecond] This system has not been enabled.");

		return result;
	}

	//! getFPS()
	/*!
	\return a const float - The FPS
	*/
	const float TimerSystem::getFPS()
	{
		if (s_enabled)
			return static_cast<float>(1.f / getStoredTime("FPS"));
		else
			ENGINE_ERROR("[TimerSystem::getFPS] This system has not been enabled.");
		return 0.f;
	}

	//! getTotalTime()
	/*!
	\return a const float - The total lifetime of the application
	*/
	const float TimerSystem::getTotalTime()
	{
		if (s_enabled)
			return getTime("TotalTime", false, true);
		else
			ENGINE_ERROR("[TimerSystem::getTotalTime] This system has not been enabled.");
		return 0.f;
	}

	//! removeTimer()
	/*!
	\param timerName a const char* - The timer name
	*/
	void TimerSystem::removeTimer(const char* timerName)
	{
		if (s_enabled)
		{
			// Check if timer exists by name
			if (timerExistsByName(timerName))
			{
				// Delete the timer and then erase from list
				delete s_timers[timerName].first;
				s_timers.erase(timerName);
			}
			else
				ENGINE_ERROR("[TimerSystem::removeTimer] Cannot find timer name {0}.", timerName);
		}
		else
			ENGINE_ERROR("[TimerSystem::removeTimer] This system has not been enabled.");
	}

	//! removeTimers
	void TimerSystem::removeTimers()
	{
		if (s_enabled)
		{
			// Delete and then clear the timer list if it is not empty
			if (s_timers.size() != 0)
			{
				for (auto& timer : s_timers)
					delete timer.second.first;
				s_timers.clear();
			}
		}
		else
			ENGINE_ERROR("[TimerSystem::removeTimers] This system has not been enabled.");
	}

	//! timerExistsByName()
	/*!
	\param timerName a const char* - The name of the timer
	\return a const bool - Has the timer been found
	*/
	const bool TimerSystem::timerExistsByName(const char* timerName)
	{
		if (s_enabled)
		{
			// Check if timer name exists in the timer list
			if (s_timers.find(timerName) != s_timers.end())
				return true;
		}
		else
			ENGINE_ERROR("[TimerSystem::timerExistsByName] This system has not been enabled.");

		return false;
	}

	//! getAllTimers()
	/*!
	\return a const std::map<const char*, std::pair<Timer*, float>>& - The list of times
	*/
	const std::map<const char*, std::pair<Timer*, float>>& TimerSystem::getAllTimers()
	{
		if(!s_enabled)
			ENGINE_ERROR("[TimerSystem::getAllTimers] This system has not been enabled.");
		return s_timers;
	}

	//! printTimers()
	void TimerSystem::printTimers()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("===================");
			ENGINE_TRACE("Timer System Timers");
			ENGINE_TRACE("===================");
			for (auto& timer : s_timers)
			{
				ENGINE_TRACE("Name: {0} - Current Time: {1}", timer.first, timer.second.first->getElapsedTime());
				ENGINE_TRACE("Name: {0} - Stored Time: {1}", timer.first, timer.second.second);
			}
			ENGINE_TRACE("===================");
		}
		else
			ENGINE_ERROR("[TimerSystem::printTimers] This system has not been enabled.");
	}
}