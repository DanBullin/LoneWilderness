/*! \file chunk.h
*
* \brief A chunk class which represents a slice of terrain
*
* \author: Daniel Bullin
*
*/
#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class Chunk
* \brief A class which represents a chunk
*/
class Chunk
{
private:
	glm::ivec2 m_chunkPosition; //!< The position of the chunk
	glm::vec3 m_chunkWorldPosition; //!< The world position of the chunk
public:
	Chunk(); //!< Constructor
	~Chunk(); //!< Destructor
	void setChunkPosition(const glm::ivec2& chunkPos, const float positionMultiplier); //!< Set the chunk's position
	glm::vec3 getWorldPositon(); //!< Get chunk's world position
};
#endif