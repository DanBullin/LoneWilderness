/*! \file shaderDataType.h
*
* \brief Shader data types
*
* \author DMU Course material
*
*/
#ifndef SHADERDATATYPE_H
#define SHADERDATATYPE_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \enum ShaderDataType
	* \brief An enum class for all shader data types we will use
	*/
	enum class ShaderDataType
	{
		None = 0, Short, Short2, Short3, Short4, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool, Byte4, FlatInt, FlatByte
	};

	namespace SDT
	{
		//! getSize()
		/*!
		\param type a ShaderDataType - The data type
		\return an uint32_t - The size in bytes of the data type
		*/
		static uint32_t getSize(ShaderDataType type)
		{
			// Return the size in bytes of these data types
			switch (type)
			{
			case ShaderDataType::Short:		return 2;
			case ShaderDataType::Short2:	return 2 * 2;
			case ShaderDataType::Short3:	return 2 * 3;
			case ShaderDataType::Short4:	return 2 * 4;
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Byte4:		return 4;
			case ShaderDataType::FlatInt:	return 4;
			case ShaderDataType::FlatByte:	return 1;
			default:						break;
			}

			return 0;
		}

		//! getST140Alignment()
		/*!
		\param type a ShaderDataType - The data type
		\return an uint32_t - The std140 layout alignment
		*/
		static uint32_t getST140Alignment(ShaderDataType type)
		{
			// Return the size in bytes of these data types
			switch (type)
			{
			case ShaderDataType::Short:		return 2;
			case ShaderDataType::Short2:	return 2 * 2;
			case ShaderDataType::Short3:	return 2 * 4;
			case ShaderDataType::Short4:	return 2 * 4;
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 4;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 4;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Byte4:		return 4;
			case ShaderDataType::FlatInt:	return 4;
			case ShaderDataType::FlatByte:	return 4;
			default:						break;
			}

			return 0;
		}

		//! getComponentCount()
		/*!
		\param type a ShaderDataType - The data type
		\return an uint32_t - The number of components for the type
		*/
		static uint32_t getComponentCount(ShaderDataType type)
		{
			// The amount of components the data type has
			switch (type)
			{
			case ShaderDataType::Short:		return 1;
			case ShaderDataType::Short2:	return 2;
			case ShaderDataType::Short3:	return 3;
			case ShaderDataType::Short4:	return 4;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3;
			case ShaderDataType::Mat4:		return 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Byte4:		return 4;
			case ShaderDataType::FlatInt:	return 1;
			case ShaderDataType::FlatByte:	return 1;
			default:						break;
			}

			return 0;
		}
	}
}
#endif