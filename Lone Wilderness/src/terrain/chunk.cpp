/*! \file chunk.cpp
*
* \brief A chunk class which represents a slice of terrain
*
* \author: Daniel Bullin
*
*/
#include "terrain/chunk.h"
#include "independent/rendering/renderers/renderer3D.h"

int Chunk::s_width; //!< The total number of tiles in the x axis
int Chunk::s_height; //!< The total number of tiles in the z axis
int Chunk::s_stepSize; //!< The size of a tile in width
Model3D* Chunk::s_model = nullptr; //!< The model of the terrain

Chunk::Chunk()
{
}

Chunk::Chunk(glm::vec3 worldPos)
{
	m_chunkWorldPosition = worldPos;
}

Chunk::~Chunk()
{
}

TerrainVertex Chunk::makeVertex(int x, int z, float xTotalLength, float zTotalLength)
{
	return { { x, 0.f, z} , { (float)x / xTotalLength, (float)z / zTotalLength } };
}


//! createGeometry
/*
\param width an int - The width of a chunk
\param height an int - The height of a chunk
\param stepSize an int - The size of a tile in the chunk
*/
void Chunk::createGeometry(int width, int height, int stepSize)
{
	s_width = width;
	s_height = height;
	s_stepSize = stepSize;

	std::vector<TerrainVertex> vertices;
	std::vector<uint32_t> indices;

	// Generating the vertices
	for (int z = 0; z < s_height; z++)
	{
		int offsetZ = z * s_stepSize;
		float zLength = static_cast<float>(s_height) * static_cast<float>(s_stepSize);

		for (int x = 0; x < width; x++)
		{
			int offsetX = x * s_stepSize;
			float xLength = static_cast<float>(width) * static_cast<float>(s_stepSize);
			vertices.push_back(makeVertex(offsetX, offsetZ, xLength, zLength));
			vertices.push_back(makeVertex(offsetX + s_stepSize, offsetZ, xLength, zLength));
			vertices.push_back(makeVertex(offsetX + s_stepSize, offsetZ + s_stepSize, xLength, zLength));
			vertices.push_back(makeVertex(offsetX, offsetZ + s_stepSize, xLength, zLength));
		}
	}

	// Generating the indices
	uint32_t count = 0;
	for (int z = 0; z < s_height; z++)
	{
		for (int x = 0; x < width; x++)
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
	ResourceManager::registerResource("Terrain", newTerrain);
	s_model = newTerrain;
}

//! onRender
/*
\param renderer a const Renderers - The renderer to use
\param renderState a const std::string& - The render state
*/
void Chunk::onRender(const Renderers renderer, const std::string& renderState)
{
	if (renderer == Renderers::Renderer3D && renderState == "Terrain")
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, { m_chunkWorldPosition.x - ((s_width * s_stepSize) / 2.f), m_chunkWorldPosition.y, m_chunkWorldPosition.z -((s_height * s_stepSize) / 2.f) });
		for (auto& mesh : s_model->getMeshes())
		{
			Renderer3D::submit("Terrain", mesh.getGeometry(), mesh.getMaterial(), model);
		}
	}
}