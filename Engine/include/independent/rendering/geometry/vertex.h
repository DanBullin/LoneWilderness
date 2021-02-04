/*! \file vertex.h
*
* \brief All geometry is defined by a set of vertices. The vertex struct defines all the information about a vertex.
* This includes: Vertex positions, texture coordinates, normals and tangents
*
* \author Daniel Bullin
*
*/
#ifndef VERTEX_H
#define VERTEX_H

#include "independent/core/common.h"
#include "independent/utils/memoryUtils.h"

namespace Engine
{
	/*! \struct Vertex3D
	* \brief A 3D vertex containing all the information about a 3D vertex
	*/
	struct Vertex3D
	{
		glm::vec3 Position; //!< Position of the vertex
		glm::vec2 TexCoords; //!< The texture coordinate of the vertex
		std::array<int16_t, 3> Normal; //!< The normal of the vertex
		std::array<int16_t, 3> Tangent; //!< The tangent of the vertex
		std::array<int16_t, 3> Bitangent; //!< The bitangent of the vertex
		Vertex3D() {}; //!< Default constructor
		Vertex3D(const glm::vec3& pos, const glm::vec2& texCoord, const std::array<int16_t, 3>& normal, const std::array<int16_t, 3>& tangent, const std::array<int16_t, 3>& bitangent)
			: Position(pos), TexCoords(texCoord), Normal(normal), Tangent(tangent), Bitangent(bitangent) {} //!< Constructor
			/*!< \param pos a const glm::vec3& - The vertex position
				 \param texCoord a const glm::vec2& - The vertex texture coordinate
				 \param normal a const std::array<int16_t, 3>& - The vertex normal
				 \param tangent a const std::array<int16_t, 3>& - The vertex tangent
				 \param bitangent a const std::array<int16_t, 3>& - The vertex bitangent */
	};

	/*! \struct Vertex2D
	* \brief A 2D vertex containing all the information about a 2D vertex
	*/
	struct Vertex2D
	{
		glm::vec4 Position; //!< Position of the vertex
		glm::vec2 TexCoords; //!< The texture coordinate of the vertex
		int32_t TexUnit; //!< The texture unit for the vertex
		uint32_t Tint; //!< The tint of the vertex
		Vertex2D() {}; //!< Default constructor
		Vertex2D(const glm::vec4& pos, const glm::vec2& texCoord, const int32_t texUnit, const glm::vec4& tint) : Position(pos), TexCoords(texCoord), TexUnit(texUnit), Tint(MemoryUtils::pack(tint)) {} //!< Constructor
			/*!< \param pos a const glm::vec4& - The vertex position
				 \param texCoord a const glm::vec2& - The vertex texture coordinate
				 \param texUnit a const int32_t - The vertex texture unit
				 \param tint a const glm::vec4& - The vertex tint */
	};
}
#endif