/*! \file destroyable.cpp
*
* \brief A class which defines whether the object which inherits it is to be deleted
*
* \author Daniel Bullin
*
*/
#include "independent/core/destroyable.h"

namespace Engine
{
	//! destroy()
	void Destroyable::destroy()
	{
		m_destroy = true;
	}

	//! getDestroyed()
	/*
	\return a const bool - Is this object to be destroyed
	*/
	const bool Destroyable::getDestroyed() const
	{
		return m_destroy;
	}
}