/*! \file eventListener.cpp
*
* \brief Responding to inputs and updates
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/eventListener.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! EventListener()
	EventListener::EventListener() : EntityComponent(ComponentType::EventListener)
	{
	}

	//! ~EventListener()
	EventListener::~EventListener()
	{
	}

	//! onAttach()
	void EventListener::onAttach()
	{
	}

	//! onDetach
	void EventListener::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void EventListener::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void EventListener::printComponentDetails()
	{
	}
}