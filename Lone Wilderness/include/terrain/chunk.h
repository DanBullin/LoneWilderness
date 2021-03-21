/*! \file chunk.h
*
* \brief A chunk class which represents a slice of terrain
*
* \author: Daniel Bullin
*
*/
#ifndef CHUNK_H
#define CHUNK_H

#include "independent/systems/systems/resourceManager.h"

using namespace Engine;

/*! \class Chunk
* \brief A class which represents a chunk
*/
class Chunk
{
private:
	static int s_width; //!< The total number of tiles in the x axis
	static int s_height; //!< The total number of tiles in the z axis
	static int s_stepSize; //!< The size of a tile in width
	static Model3D* s_model; //!< The model of the terrain

	glm::vec3 m_chunkPosition; //!< The position of the chunk
	glm::vec3 m_chunkWorldPosition; //!< The world position of the chunk

	static TerrainVertex makeVertex(int x, int z, float xTotalLength, float zTotalLength);
public:
	Chunk(); //!< Constructor
	Chunk(glm::vec3 worldPos); //!< Constructor
	~Chunk(); //!< Destructor

	static void createGeometry(int width, int height, int stepSize);

	void onRender(const Renderers renderer, const std::string& renderState); //!< On Render
};
#endif