/*! \file chunk.cpp
*
* \brief A chunk class which represents a slice of terrain
*
* \author: Daniel Bullin
*
*/
#include "terrain/chunk.h"

//! Chunk()
Chunk::Chunk()
{
}

//! ~Chunk()
Chunk::~Chunk()
{
}

//! setChunkPosition()
/*
\param chunkPos a const glm::ivec2& - The chunk position
\param positionMultiplier a const float - The position multiplier
*/
void Chunk::setChunkPosition(const glm::ivec2& chunkPos, const float positionMultiplier)
{
	m_chunkPosition = chunkPos;
	m_chunkWorldPosition = { static_cast<float>(m_chunkPosition.x) * positionMultiplier, 50.f, static_cast<float>(m_chunkPosition.y) * positionMultiplier };
}

//! getWorldPosition
/*
\return a glm::vec3 - The chunk's world position
*/
glm::vec3 Chunk::getWorldPositon()
{
	return m_chunkWorldPosition;
}