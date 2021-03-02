/*! \file timer.h 
*
* \brief A class to measure time
*
* \author DMU Course material
*
*/
#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

namespace Engine
{
	/*! \class Timer
	* \brief Interface class for timers
	*/
	class Timer {
	public:
		virtual ~Timer(); //!< Destructor
		virtual void start() = 0; //!< Start the timer
		virtual void reset() = 0; //!< Reset the timer
		virtual const float getElapsedTime() = 0; //!< Gets the time elapsed since the last start or reset
			/*!< \return a const float - The elapsed time */
	};

	/*! \class ChronoTimer
	* \brief A choro timer class inheriting from timer
	*/
	class ChronoTimer : public Timer {
	private:
		std::string m_timerName; //!< The name of the timer
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint; //!< Start time for the timer
		std::chrono::time_point<std::chrono::high_resolution_clock> m_endPoint; //!< End time for the timer
	public:
		ChronoTimer(const char* timerName); //!< Constructor
		virtual ~ChronoTimer(); //!< Destructor
		virtual inline void start() override { m_startPoint = std::chrono::high_resolution_clock::now(); } //!< Start the chrono timer
		virtual inline void reset() override { m_startPoint = std::chrono::high_resolution_clock::now(); } //!< Stop the chrono timer
		virtual const float getElapsedTime() override; //!< Get the current elapsed time
	};
}
#endif