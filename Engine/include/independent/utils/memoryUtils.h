/*! \file memoryUtils.h
*
* \brief A memory utility class which performs various helpful functions in regards to memory. Such as normalising data or packing
*
* \author Daniel Bullin
*
*/
#ifndef MEMORYUTILS_H
#define MEMORYUTILS_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \class MemoryUtils
	* \brief A utility class to help with memory optimizations
	*/
	class MemoryUtils
	{
	public:
		static std::array<int16_t, 3> normalise(const glm::vec3& data); //!< Normalise a glm::vec3 to short
		static std::array<int16_t, 2> normalise(const glm::vec2& data); //!< Normalise a glm::vec2 to short
		static uint32_t pack(const glm::vec4& data); //!< Pack a glm::vec4 into an uint_32t
		static uint32_t pack(const glm::vec3& data); //!< Pack a glm::vec3 into an uint_32t
	};
}
#endif