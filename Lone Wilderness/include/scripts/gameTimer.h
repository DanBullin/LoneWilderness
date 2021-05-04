/*! \file gameTimer.h
*
* \brief A game timer script
*
* \author: Daniel Bullin
*
*/
#ifndef GAMETIMER_H
#define GAMETIMER_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class GameTimer
* \brief A game timer script
*/
class GameTimer : public NativeScript
{
private:
	bool m_enabled; //!< Is the timer running
	float m_time; //!< The time
public:
	GameTimer(); //!< Constructor
	~GameTimer(); //!< Destructor

	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update

	void start(); //!< Start the timer
	void stop(); //!< Stop the timer
	void reset(); //!< Reset the timer
	bool isEnabled(); //!< Is the timer enabled
	const float getTime(); //!< Get the time
};
#endif