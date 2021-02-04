/*! \file mesh3D.cpp
*
* \brief A 3D mesh class containing all the geometry information about the mesh in a batched rendering system
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/log.h"
#include "independent/rendering/geometry/mesh3D.h"

namespace Engine
{
	//! Mesh3D()
	/*!
	\param geometry a const Geometry3D& - A reference to the geometry data
	*/
	Mesh3D::Mesh3D(const Geometry3D& geometry)
		: m_geometry(geometry)
	{
	}

	//! ~Mesh3D()
	Mesh3D::~Mesh3D()
	{
	}

	//! getGeometry()
	/*!
	\return a const Geometry3D& - The mesh's geometry
	*/
	const Geometry3D& Mesh3D::getGeometry() const
	{
		return m_geometry;
	}
}