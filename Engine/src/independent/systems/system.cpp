/*! \file system.cpp
*
* \brief The base functionality of a system
*
* \author Daniel Bullin
*
*/

#include "independent/systems/system.h"

namespace Engine
{
	//! System()
	/*!
	\param type a const Systems::Type - The type of the system
	*/
	System::System(const Systems::Type type) : m_systemType(type)
	{
	}

	//! ~System
	System::~System()
	{
	}

	//! getSystemType()
	/*!
	\return a const Systems::Type - The type of the system
	*/
	const Systems::Type System::getSystemType() const
	{
		return m_systemType;
	}
}