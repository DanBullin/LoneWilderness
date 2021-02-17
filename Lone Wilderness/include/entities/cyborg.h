/*! \file cyborg.h
*
* \brief A Cyborg dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef CYBORG_H
#define CYBORG_H

#include "independent/entities/entity.h"

using namespace Engine;

/*! \class Cyborg
* \brief A cyborg dummy class
*/
class Cyborg : public Entity
{
public:
	Cyborg(); //!< Constructor
	~Cyborg(); //!< Destructor
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif