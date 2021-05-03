/*! \file chunkManager.cpp
*
* \brief A chunk manager class which manages chunks
*
* \author: Daniel Bullin
*
*/
#include "terrain/chunkManager.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/systems/systems/log.h"

#define CHUNKSIZE 10
#define CHUNKSTEPSIZE 5

std::map<std::pair<int, int>, Chunk*> ChunkManager::s_chunks; //!< A vector of all the chunks loaded
int ChunkManager::s_chunksSize; //!< The number of chunks layers around the player's current chunk
int ChunkManager::s_chunkSize; //!< The total number of tiles in any axis
int ChunkManager::s_chunkStepSize; //!< The size of a tile in width
Model3D* ChunkManager::s_model; //!< The model of the terrain

//! makeVertex()
/*
\param x an int - The x position
\param z an int - The z position
\param xTotalLength - The total x length
\param zTotalLength - The total z length
*/
TerrainVertex ChunkManager::makeVertex(int x, int z, float xTotalLength, float zTotalLength)
{
	return { { x, 0.f, z} , { (float)x / xTotalLength, (float)z / zTotalLength } };
}

//! ChunkManager()
ChunkManager::ChunkManager()
{
}

//! ~ChunkManager()
ChunkManager::~ChunkManager()
{
	deleteChunks();
}

//! start()
void ChunkManager::start()
{
	createGeometry(CHUNKSIZE, CHUNKSTEPSIZE);
	s_chunksSize = 3;
}

//! createGeometry
/*
\param chunkSize an int - The size of a chunk
\param stepSize an int - The size of a tile in the chunk
*/
void ChunkManager::createGeometry(int chunkSize, int stepSize)
{
	s_chunkSize = chunkSize;
	s_chunkStepSize = stepSize;

	std::vector<TerrainVertex> vertices;
	std::vector<uint32_t> indices;

	// Generating the vertices
	for (int z = 0; z < s_chunkSize; z++)
	{
		int offsetZ = z * s_chunkStepSize;
		float zLength = static_cast<float>(s_chunkSize) * static_cast<float>(s_chunkStepSize);

		for (int x = 0; x < s_chunkSize; x++)
		{
			int offsetX = x * s_chunkStepSize;
			float xLength = static_cast<float>(s_chunkSize) * static_cast<float>(s_chunkStepSize);
			vertices.push_back(makeVertex(offsetX, offsetZ, xLength, zLength));
			vertices.push_back(makeVertex(offsetX + s_chunkStepSize, offsetZ, xLength, zLength));
			vertices.push_back(makeVertex(offsetX + s_chunkStepSize, offsetZ + s_chunkStepSize, xLength, zLength));
			vertices.push_back(makeVertex(offsetX, offsetZ + s_chunkStepSize, xLength, zLength));
		}
	}

	// Generating the indices
	uint32_t count = 0;
	for (int z = 0; z < s_chunkSize; z++)
	{
		for (int x = 0; x < s_chunkSize; x++)
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

//! deleteChunks()
void ChunkManager::deleteChunks()
{
	for (auto& chunk : s_chunks)
	{
		if (chunk.second)
			delete chunk.second;
	}
	s_chunks.clear();
}

//! setChunksSize()
/*
\param size a const int - The number of layers around the player's chunk
*/
void ChunkManager::setChunksSize(const int size)
{
	s_chunksSize = size;
	deleteChunks();
}

//! getChunksSize()
/*
\return an int - The number of chunk layer's around the player's chunk
*/
int ChunkManager::getChunksSize()
{
	return s_chunksSize;
}

//! updateChunks
/*
\param playerPos a const glm::ivec2& - The player's position
*/
void ChunkManager::updateChunks(const glm::ivec2& playerPos)
{
	int currentChunkX = static_cast<int>(floor(static_cast<float>(playerPos.x) / (static_cast<float>(s_chunkSize) * static_cast<float>(s_chunkStepSize))));
	int currentChunkZ = static_cast<int>(floor(static_cast<float>(playerPos.y) / (static_cast<float>(s_chunkSize) * static_cast<float>(s_chunkStepSize))));

	for (int i = -s_chunksSize; i <= s_chunksSize; i++)
	{
		for (int j = -s_chunksSize; j <= s_chunksSize; j++)
		{
			if (!s_chunks[{i, j}])
				s_chunks[{i, j}] = new Chunk;

			s_chunks[{i, j}]->setChunkPosition({ currentChunkX + j, currentChunkZ + i }, static_cast<float>((s_chunkSize * s_chunkStepSize)));
		}
	}
}

//! onRender
/*
\param renderer a const Renderers - The renderer to use
\param renderState a const std::string& - The render state
*/
void ChunkManager::onRender(const Renderers renderer, const std::string& renderState)
{
	if (renderer == Renderers::Renderer3D && renderState == "Terrain")
	{
		for (auto& chunk : s_chunks)
		{
			glm::mat4 model = glm::mat4(1.f);
			glm::vec3 worldPos = chunk.second->getWorldPositon();

			model = glm::translate(model, { worldPos.x, worldPos.y, worldPos.z });
			for (auto& mesh : s_model->getMeshes())
			{
				Renderer3D::submit("Terrain", mesh.getGeometry(), mesh.getMaterial(), model);
			}
		}
	}
}