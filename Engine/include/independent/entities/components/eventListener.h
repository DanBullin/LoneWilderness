/*! \file eventListener.h
*
* \brief A component which makes the entity respond to inputs and updates
*
* \author Daniel Bullin
*
*/
#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "independent/entities/entityComponent.h"

namespace Engine
{
	/*! \class EventListener
	* \brief A class which responds to inputs and updates
	*/
	class EventListener : public EntityComponent
	{
	public:
		EventListener(); //!< Default constructor
		~EventListener(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the event listener
		void printComponentDetails() override; //!< Print component details
	};
}
#endif