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

	//! Mesh3D()
	/*!
	\param vertices a std::vector<Vertex3D>& - A list of vertices
	\param indices a std::vector<uint32_t>& - A list of indices
	*/
	Mesh3D::Mesh3D(std::vector<Vertex3D>& vertices, std::vector<uint32_t> indices) : m_vertices(vertices), m_indices(indices)
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

	//! getGeometryRef()
	/*!
	\return a Geometry3D& - The mesh's geometry
	*/
	Geometry3D& Mesh3D::getGeometryRef()
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

	//! getVertices()
	/*!
	\return a std::vector<Vertex3D>& - A list of vertices
	*/
	std::vector<Vertex3D>& Mesh3D::getVertices()
	{
		return m_vertices;
	}

	//! getIndices()
	/*!
	\return a std::vector<uint32_t>& - A list of indices
	*/
	std::vector<uint32_t>& Mesh3D::getIndices()
	{
		return m_indices;
	}
}