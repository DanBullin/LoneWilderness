/*! \file mathUtils.cpp
*
* \brief A math utility class to help with data regarding maths
*
* \author Daniel Bullin
*
*/
#include "independent/utils/mathUtils.h"

namespace Engine
{
	//! convertMat4ToArray()
	/*!
	\param data a const glm::mat4& - The data to convert
	\return a std::array<double, 16> - The matrix data in array format
	*/
	std::array<double, 16> MathUtils::convertMat4ToArray(const glm::mat4& data)
	{
		std::array<double, 16> matArray;

		const float *pSource = (const float*)glm::value_ptr(data);
		for (int i = 0; i < 16; ++i)
			matArray[i] = pSource[i];
		return matArray;
	}
}