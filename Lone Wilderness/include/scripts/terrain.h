/*! \file terrain.h
*
* \brief A terrain script class which will create a terrain
*
* \author: Daniel Bullin
*
*/
#ifndef TERRAIN_H
#define TERRAIN_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/transform.h"
#include "terrain/chunkManager.h"

using namespace Engine;

/*! \class Terrain
* \brief A terrain script
*/
class Terrain : public NativeScript
{
private:
	static ChunkManager* s_chunkManager; //!< A chunk manager

	UniformBuffer* m_tessUBO; //!< The tessellation UBO
	bool m_drawWireframe; //!< Draw the terrain in wireframe
	uint32_t m_tessellationEquation; //!< The equation to use for tessellation
	bool m_generateY; //!< Generate the y valus for generation
	uint32_t m_octaves; //!< The number of octaves
	float m_scale; //!< The scale factor for the terrain
	float m_frequency; //!< The frequency
	float m_amplitude; //!< The amplitude
	float m_amplitudeDivisor; //!< The amplitude divisor
	float m_frequencyMultiplier; //!< The frequency multiplier

	Transform* m_playerTransform; //!< The player's transform
public:
	Terrain(); //!< Constructor
	~Terrain(); //!< Destructor

	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onRender(const Renderers renderer, const std::string& renderState); //!< On Render
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif