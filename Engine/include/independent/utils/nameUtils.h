/*! \file nameUtils.h
*
* \brief A name utility class which performs various helpful functions in regards to strings & names.
*
* \author Joshua Simons
*
*/
#ifndef NAMEUTILS_H
#define NAMEUTILS_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \class NameUtils
	* \brief A utility class to help with name strings
	*/
	class NameUtils
	{
	public:
		const static char m_start = '('; //!< Start character that encapsulates the number on the end of the name (Goes on the front of the number)
		const static char m_end = ')'; //!< End character that encapsulates the number on the end of the name (Goes on the back of the number)
		static std::string rename(const std::string& name); //!< Attach a number to the end of the name
	};
}
#endif