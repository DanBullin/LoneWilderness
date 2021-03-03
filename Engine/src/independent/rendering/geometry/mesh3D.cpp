/*! \file mesh3D.cpp
*
* \brief A 3D mesh class containing all the geometry information about the mesh in a batched rendering system
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/geometry/mesh3D.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! Mesh3D()
	/*!
	\param geometry a const Geometry3D& - A reference to the geometry data
	*/
	Mesh3D::Mesh3D(const Geometry3D& geometry) : m_geometry(geometry)
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

	//! getMaterial()
	/*
	\return a Material* - A pointer to the material
	*/
	Material* Mesh3D::getMaterial()
	{
		return m_material;
	}

	//! setMaterial()
	/*
	\param material a Material* - A pointer to the material
	*/
	void Mesh3D::setMaterial(Material* material)
	{
		if (material)
			m_material = material;
		else
			ENGINE_ERROR("[Mesh3D::setMaterial] The material provided is an invalid material.");
	}
}