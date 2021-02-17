/*! \file mesh3D.h
*
* \brief A 3D mesh class containing all the geometry information about the mesh in a batched rendering system
*
* \author Daniel Bullin
*
*/
#ifndef MESH3D_H
#define MESH3D_H

#include "independent/core/common.h"
#include "independent/rendering/geometry/vertex.h"
#include "independent/rendering/geometry/vertexBuffer.h"

namespace Engine
{
	/*! \struct Geometry3D
	* \brief A piece of 3D geometry and its position in the buffer
	*/
	struct Geometry3D
	{
		uint32_t ID; //!< Unique ID
		uint32_t VertexCount; //!< The vertex count of this 3D mesh
		uint32_t IndexCount; //!< The index count of this 3D mesh
		uint32_t FirstVertex; //!< The index of the first vertex
		uint32_t FirstIndex; //!< The index of the first index
		VertexBuffer* VertexBuffer; //!< A pointer to the vertex buffer which fits the format of the type of vertex used
	};

	/*! \class Mesh3D
	* \brief A 3D mesh class that contains the geometry
	*/
	class Mesh3D
	{
	private:
		Geometry3D m_geometry; //!< The geometry of this 3D mesh
	public:
		Mesh3D(const Geometry3D& geometry); //!< Constructor
		~Mesh3D(); //!< Destructor

		const Geometry3D& getGeometry() const; //!< Get the geometry
	};
}
#endif