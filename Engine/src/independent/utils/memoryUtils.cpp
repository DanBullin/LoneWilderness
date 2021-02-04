/*! \file memoryUtils.cpp
*
* \brief A memory utility class which performs various helpful functions in regards to memory. Such as normalising data or packing
*
* \author Daniel Bullin
*
*/

#include "independent/utils/memoryUtils.h"

namespace Engine
{
	//! normalise()
	/*!
	\param data a const glm::vec3& - The data to normalise
	\return a std::array<int16_t, 3> - The normalised data
	*/
	std::array<int16_t, 3> MemoryUtils::normalise(const glm::vec3& data)
	{
		std::array<int16_t, 3> result;

		if (data.x == 1.0f) result.at(0) = INT16_MAX;
		else if (data.x == -1.0f) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(data.x * static_cast<float>(INT16_MAX));

		if (data.y == 1.0f) result.at(1) = INT16_MAX;
		else if (data.y == -1.0f) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(data.y * static_cast<float>(INT16_MAX));

		if (data.z == 1.0f) result.at(2) = INT16_MAX;
		else if (data.z == -1.0f) result.at(2) = INT16_MIN;
		else result.at(2) = static_cast<int16_t>(data.z * static_cast<float>(INT16_MAX));

		return result;
	}

	//! normalise()
	/*!
	\param data a const glm::vec2& - The data to normalise
	\return a std::array<int16_t, 2> - The normalised data
	*/
	std::array<int16_t, 2> MemoryUtils::normalise(const glm::vec2& data)
	{
		std::array<int16_t, 2> result;

		if (data.x == 1.0f) result.at(0) = INT16_MAX;
		else if (data.x == -1.0f) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(data.x * static_cast<float>(INT16_MAX));

		if (data.y == 1.0f) result.at(1) = INT16_MAX;
		else if (data.y == -1.0f) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(data.y * static_cast<float>(INT16_MAX));

		return result;
	}

	//! pack()
	/*!
	\param data a const glm::vec4& - The data to pack
	\return an uint32_t - The packed data
	*/
	uint32_t MemoryUtils::pack(const glm::vec4& data)
	{
		uint32_t result = 0; // 000000000 000000000 000000000 00000000
		// Format: AAAAAAAA BBBBBBBB GGGGGGGG RRRRRRRR
		uint32_t r = (static_cast<uint32_t>(data.r * 255.0f)) << 0;
		uint32_t g = (static_cast<uint32_t>(data.g * 255.0f)) << 8;
		uint32_t b = (static_cast<uint32_t>(data.b * 255.0f)) << 16;
		uint32_t a = (static_cast<uint32_t>(data.a * 255.0f)) << 24;
		result = (r | g | b | a);
		return result;
	}

	//! pack()
	/*!
	\param data a const glm::vec3& - The data to pack
	\return an uint32_t - The packed data
	*/
	uint32_t MemoryUtils::pack(const glm::vec3& data)
	{
		return pack({ data.x, data.y, data.z, 1.f });
	}
}