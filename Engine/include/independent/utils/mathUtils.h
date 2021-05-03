/*! \file mathUtils.h
*
* \brief A math utility class to help with data regarding maths
*
* \author Daniel Bullin
*
*/
#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \class MathUtils
	* \brief A utility class to help with maths
	*/
	class MathUtils
	{
	public:
		static std::array<double, 16> convertMat4ToArray(const glm::mat4& data); //!< Convert a mat4 to array of size 16
		static glm::mat4 getModelMatrix(const glm::vec3& pos); //!< Get a model matrix using position only
		static glm::mat4 getModelMatrix(const glm::vec3& pos, const glm::vec3& scale); //!< Get a model matrix using position and scale only
	};
}
#endif