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
	/*! \struct DirectionalLightSDT
	* \brief A directional light shader data type
	*/
	struct DirectionalLightSDT {
		glm::vec4 direction;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
	};

	/*! \struct PointLightSDT
	* \brief A point light shader data type
	*/
	struct PointLightSDT {
		glm::vec4 position;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		float constant;
		float linear;
		float quadratic;
		float padding;
	};

	/*! \struct SpotLightSDT
	* \brief A spot light shader data type
	*/
	struct SpotLightSDT {
		glm::vec4 position;
		glm::vec4 direction;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		float cutOff;
		float outerCutOff;
		float constant;
		float linear;
		float quadratic;
		float padding1;
		float padding2;
		float padding3;
	};

	/*! \enum ShaderDataType
	* \brief An enum class for all shader data types we will use
	*/
	enum class ShaderDataType
	{
		None = 0, Short, Short2, Short3, Short4, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool, Byte4, FlatInt, FlatByte,
		DirectionalLightSDT, PointLightSDT, SpotLightSDT
	};

	namespace SDT
	{
		//! convertStringToSDT()
		/*!
		\param type a const std::string& - The type as a string literal
		\return a ShaderDataType - The shader data type
		*/
		static ShaderDataType convertStringToSDT(const std::string& type)
		{
			if (type == "Short") return ShaderDataType::Short;
			if (type == "Short2") return ShaderDataType::Short2;
			if (type == "Short3") return ShaderDataType::Short3;
			if (type == "Short4") return ShaderDataType::Short4;
			if (type == "Float") return ShaderDataType::Float;
			if (type == "Float2") return ShaderDataType::Float2;
			if (type == "Float3") return ShaderDataType::Float3;
			if (type == "Float4") return ShaderDataType::Float4;
			if (type == "Mat3") return ShaderDataType::Mat3;
			if (type == "Mat4") return ShaderDataType::Mat4;
			if (type == "Int") return ShaderDataType::Int;
			if (type == "Int2") return ShaderDataType::Int2;
			if (type == "Int3") return ShaderDataType::Int3;
			if (type == "Int4") return ShaderDataType::Int4;
			if (type == "Bool") return ShaderDataType::Bool;
			if (type == "Byte4") return ShaderDataType::Byte4;
			if (type == "FlatInt") return ShaderDataType::FlatInt;
			if (type == "FlatByte") return ShaderDataType::FlatByte;
			if (type == "DirectionalLight") return ShaderDataType::DirectionalLightSDT;
			if (type == "PointLight") return ShaderDataType::PointLightSDT;
			if (type == "SpotLight") return ShaderDataType::SpotLightSDT;
			else return ShaderDataType::None;
		}

		//! convertSDTToString()
		/*!
		\param type a const ShaderDataType - The shader data type
		\return a std::string - The type as a string literal
		*/
		static std::string convertSDTToString(const ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::Short:
					return "Short";
				case ShaderDataType::Short2:
					return "Short2";
				case ShaderDataType::Short3:
					return "Short3";
				case ShaderDataType::Short4:
					return "Short4";
				case ShaderDataType::Float:
					return "Float";
				case ShaderDataType::Float2:
					return "Float2";
				case ShaderDataType::Float3:
					return "Float3";
				case ShaderDataType::Float4:
					return "Float4";
				case ShaderDataType::Mat3:
					return "Mat3";
				case ShaderDataType::Mat4:
					return "Mat4";
				case ShaderDataType::Int:
					return "Int";
				case ShaderDataType::Int2:
					return "Int2";
				case ShaderDataType::Int3:
					return "Int3";
				case ShaderDataType::Int4:
					return "Int4";
				case ShaderDataType::Bool:
					return "Bool";
				case ShaderDataType::Byte4:
					return "Byte4";
				case ShaderDataType::FlatInt:
					return "FlatInt";
				case ShaderDataType::FlatByte:
					return "FlatByte";
				case ShaderDataType::DirectionalLightSDT:
					return "DirectionalLightSDT";
				case ShaderDataType::PointLightSDT:
					return "PointLightSDT";
				case ShaderDataType::SpotLightSDT:
					return "SpotLightSDT";
				default:
					return "None";
			}
		}

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
			case ShaderDataType::DirectionalLightSDT:	return 64;
			case ShaderDataType::PointLightSDT:	return 80;
			case ShaderDataType::SpotLightSDT:	return 112;
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
			case ShaderDataType::Bool:		return 4;
			case ShaderDataType::Byte4:		return 4;
			case ShaderDataType::FlatInt:	return 4;
			case ShaderDataType::FlatByte:	return 4;
			case ShaderDataType::DirectionalLightSDT:	return 64;
			case ShaderDataType::PointLightSDT:	return 80;
			case ShaderDataType::SpotLightSDT:	return 112;
			default:						break;
			}

			return 0;
		}

		//! getAlignmentOffset()
		/*!
		\param type a ShaderDataType - The data type
		\return an uint32_t - The aligned offset according to std140
		*/
		static uint32_t getAlignmentOffset(ShaderDataType type)
		{
			// Return the offset according to std140
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
				case ShaderDataType::Mat3:		return 4 * 4;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 4;
				case ShaderDataType::Int2:		return 4 * 2;
				case ShaderDataType::Int3:		return 4 * 4;
				case ShaderDataType::Int4:		return 4 * 4;
				case ShaderDataType::Bool:		return 4;
				case ShaderDataType::Byte4:		return 4;
				case ShaderDataType::FlatInt:	return 4;
				case ShaderDataType::FlatByte:	return 4;
				case ShaderDataType::DirectionalLightSDT:	return 64;
				case ShaderDataType::PointLightSDT:	return 80;
				case ShaderDataType::SpotLightSDT:	return 112;
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