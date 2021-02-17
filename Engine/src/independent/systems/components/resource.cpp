/*! \file resource.cpp
*
* \brief A resource that holds some data
*
* \author Daniel Bullin
*
*/
#include "independent/systems/components/resource.h"

namespace Engine
{
	//! Resource()
	/*!
	\param resourceName a const std::string& - The name of the resource
	\param type a const ResourceType - The resource type
	*/
	Resource::Resource(const std::string& resourceName, const ResourceType type) : m_name(resourceName), m_type(type)
	{
	}

	//! increaseCount()
	void Resource::increaseCount()
	{
		// Increase by one, call this function when the referencing entity is created
		m_referenceCounter++;
	}

	//! decreaseCount()
	void Resource::decreaseCount()
	{
		// Decrease by one, call this function when the referencing entity is deleted
		m_referenceCounter--;
	}

	//! getName()
	/*!
	\return a const std::string& - The name of the resource
	*/
	const std::string& Resource::getName() const
	{
		return m_name;
	}

	//! getReferenceCount()
	/*!
	\return a const uint32_t - The number of external references to this resource
	*/
	const uint32_t Resource::getReferenceCount() const
	{
		// If this count reaches 0, the resource should be deleted
		// This will be handled elsewhere at an appropriate time
		return m_referenceCounter;
	}

	//! getType()
	/*!
	\return a ResourceType - The resource type
	*/
	ResourceType Resource::getType() const
	{
		return m_type;
	}
}