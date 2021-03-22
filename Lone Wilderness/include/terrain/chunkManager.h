/*! \file chunkManager.h
*
* \brief A chunk manager class which manages chunks
*
* \author: Daniel Bullin
*
*/
#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "independent/systems/systems/resourceManager.h"
#include "chunk.h"

using namespace Engine;

/*! \class ChunkManager
* \brief A class which manages chunks
*/
class ChunkManager
{
private:
	static std::map<std::pair<int, int>, Chunk*> s_chunks; //!< A vector of all the chunks loaded
	static int s_chunksSize; //!< The number of chunks layers around the player's current chunk
	static int s_chunkSize; //!< The total number of tiles in any axis
	static int s_chunkStepSize; //!< The size of a tile in width
	static Model3D* s_model; //!< The model of the terrain

	static TerrainVertex makeVertex(int x, int z, float xTotalLength, float zTotalLength); //!< Make a new vertex
public:
	ChunkManager(); //!< Constructor
	~ChunkManager(); //!< Destructor

	static void start(); //!< Start the chunk manager
	static void createGeometry(int chunkSize, int stepSize); //!< Create the chunk geometry

	static void deleteChunks(); //!< Delete chunks
	static void setChunksSize(const int size); //!< Set the number of chunks along an axis
	static int getChunksSize(); //!< Get the number of chunks along an axis

	static void updateChunks(const glm::ivec2& playerPos); //!< Update all the chunks

	void onRender(const Renderers renderer, const std::string& renderState); //!< On Render
};
#endif