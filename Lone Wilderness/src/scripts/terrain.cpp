/*! \file terrain.cpp
*
* \brief A terrain script class which will create a terrain
*
* \author: Daniel Bullin
*
*/
#include "scripts/terrain.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/renderers/renderer3D.h"

TerrainVertex Terrain::makeVertex(int x, int z, float xTotalLength, float zTotalLength)
{
	return { { x, 0.f, z} , { (float)x / xTotalLength, (float)z / zTotalLength } };
}

//! Terrain()
Terrain::Terrain()
{
	m_model = nullptr;
	m_width = 50;
	m_height = 50;
	m_stepSize = 10;
}

//! ~Terrain()
Terrain::~Terrain()
{
	if (m_model) delete m_model;
}

//! onAttach()
void Terrain::onAttach()
{
	std::vector<TerrainVertex> vertices;
	std::vector<uint32_t> indices;

	// Generating the vertices
	for (int z = 0; z < m_height; z++)
	{
		int offsetZ = z * m_stepSize;
		float zLength = static_cast<float>(m_height) * static_cast<float>(m_stepSize);

		for (int x = 0; x < m_width; x++)
		{
			int offsetX = x * m_stepSize;
			float xLength = static_cast<float>(m_width) * static_cast<float>(m_stepSize);
			vertices.push_back(makeVertex(offsetX, offsetZ, xLength, zLength));
			vertices.push_back(makeVertex(offsetX + m_stepSize, offsetZ, xLength, zLength));
			vertices.push_back(makeVertex(offsetX + m_stepSize, offsetZ + m_stepSize, xLength, zLength));
			vertices.push_back(makeVertex(offsetX, offsetZ + m_stepSize, xLength, zLength));
		}
	}

	// Generating the indices
	uint32_t count = 0;
	for (int z = 0; z < m_height; z++)
	{
		for (int x = 0; x < m_width; x++)
		{
			indices.push_back(count);
			indices.push_back(count + 1);
			indices.push_back(count + 3);
			indices.push_back(count + 1);
			indices.push_back(count + 2);
			indices.push_back(count + 3);
			count += 4;
		}
	}

	// Create a piece of geometry using local vertices and indices information
	Geometry3D geometry;
	geometry.VertexBuffer = ResourceManager::getResource<VertexBuffer>("TerrainVertexBuffer");
	Renderer3D::addGeometry(vertices, indices, geometry);

	Model3D* newTerrain = new Model3D("Terrain");
	newTerrain->getMeshes().push_back(Mesh3D(geometry));
	newTerrain->getMeshes().at(0).setMaterial(ResourceManager::getResource<Material>("TerrainMaterial"));
	m_model = newTerrain;
}

void Terrain::onRender(const Renderers renderer)
{
	if (renderer == Renderers::Renderer3D)
	{
		for (auto& mesh : m_model->getMeshes())
		{
			Renderer3D::submit("Terrain", mesh.getGeometry(), mesh.getMaterial(), getParent()->getComponent<Transform>()->getModelMatrix());
		}
	}
}