/*! \file timerSystem.h
*
* \brief A timer system which manages timers and stores recorded times
*
* \author Daniel Bullin
*
*/
#ifndef TIMERSYSTEM_H
#define TIMERSYSTEM_H

#include "independent/core/common.h"
#include "independent/systems/system.h"
#include "independent/systems/components/timer.h"

namespace Engine 
{
	/*! \class TimerSystem
	* \brief A timer system which handles timers and stores recorded times
	*/
	class TimerSystem : public System {
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::map<const char*, std::pair<Timer*, float>> s_timers; //!< All running timers
	public:
		TimerSystem(); //!< Constructor
		~TimerSystem(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static void startTimer(const char* timerName); //!< Start a new timer with name
		static void stopTimer(const char* timerName, const bool deleteTimer, const bool storeResult = false); //!< Stop the timer
		static const float getTime(const char* timerName, const bool reset, const bool storeResult = true); //!< Get the result of a timer
		static const float getStoredTime(const char* timerName); //!< Get the stored result of a timer
		static const float getTimePerSecond(const char* timerName, const bool reset, const bool storeResult = true); //!< Get the time per second (used for FPS)

		static const float getFPS(); //!< Simplifies getting the FPS
		static const float getTotalTime(); //!< Simplifies getting the total application lifetime

		static void removeTimer(const char* timerName); //!< Remove a timer from the list and its associated results
		static void removeTimers(); //!< Remove all timers

		static const bool timerExistsByName(const char* timerName); //!< Check if a timer exists by name
		static const std::map<const char*, std::pair<Timer*, float>>& getAllTimers(); //!< Get a list of all the recorded times

		static void printTimers(); //!< Print all timers
	};
}
#endif