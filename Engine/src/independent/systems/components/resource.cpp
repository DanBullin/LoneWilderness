/*! \file resource.cpp
*
* \brief A resource that holds some data
*
* \author Daniel Bullin
*
*/
#include "independent/systems/components/resource.h"
#include "independent/systems/systems/log.h"

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

	//! ~Resource()
	Resource::~Resource()
	{
	}

	//! getName()
	/*!
	\return a const std::string& - The name of the resource
	*/
	const std::string& Resource::getName() const
	{
		return m_name;
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