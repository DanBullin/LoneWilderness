/*! \file terrain.cpp
*
* \brief A terrain script class which will create a terrain
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/terrain.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer3D.h"

ChunkManager* Terrain::s_chunkManager = nullptr;

//! Terrain()
Terrain::Terrain()
{
	s_chunkManager = new ChunkManager;
	s_chunkManager->start();

	m_tessUBO = ResourceManager::getResource<UniformBuffer>("TessellationUBO");
	m_drawWireframe = false;
	m_tessellationEquation = 1;
	m_generateY = true;
	m_scale = 100.f;
	m_octaves = 10;
	m_frequency = 0.005f;
	m_amplitude = 100.f;
	m_amplitudeDivisor = 2.f;
	m_frequencyMultiplier = 2.f;
	m_playerTransform = nullptr;

}

//! ~Terrain()
Terrain::~Terrain()
{
	if (s_chunkManager)
		delete s_chunkManager;

	s_chunkManager = nullptr;
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Terrain::onPostUpdate(const float timestep, const float totalTime)
{
	if (!m_playerTransform) m_playerTransform = getParent()->getParentScene()->getEntity("Player1")->getComponent<Transform>();

	glm::vec3 playerPos = m_playerTransform->getWorldPosition();
	s_chunkManager->updateChunks({ playerPos.x, playerPos.z });
}

//! onRender
/*
\param renderer a const Renderers - The renderer to use
\param renderState a const std::string& - The render state
*/
void Terrain::onRender(const Renderers renderer, const std::string& renderState)
{
	if (renderer == Renderers::Renderer3D && renderState == "Terrain")
	{
		m_tessUBO->uploadData("u_tessellationEquation", static_cast<void*>(&m_tessellationEquation));
		m_tessUBO->uploadData("u_generateY", static_cast<void*>(&m_generateY));
		m_tessUBO->uploadData("u_scale", static_cast<void*>(&m_scale));
		m_tessUBO->uploadData("u_octaves", static_cast<void*>(&m_octaves));
		m_tessUBO->uploadData("u_frequency", static_cast<void*>(&m_frequency));
		m_tessUBO->uploadData("u_amplitude", static_cast<void*>(&m_amplitude));
		m_tessUBO->uploadData("u_amplitudeDivisor", static_cast<void*>(&m_amplitudeDivisor));
		m_tessUBO->uploadData("u_frequencyMultiplier", static_cast<void*>(&m_frequencyMultiplier));
		if (m_drawWireframe) RenderUtils::enableWireframe(true);

		// Draw all chunks
		s_chunkManager->onRender(renderer, renderState);
	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Terrain::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::Z)
	{
		m_drawWireframe = !m_drawWireframe;
	}

	if (e.getKeyCode() == Keys::X)
	{
		m_tessellationEquation = !m_tessellationEquation;
	}

	if (e.getKeyCode() == Keys::C)
	{
		m_generateY = !m_generateY;
	}

	if (e.getKeyCode() == Keys::N)
	{
		m_scale += 10.f;
	}

	if (e.getKeyCode() == Keys::M)
	{
		m_scale -= 10.f;
	}

	if (e.getKeyCode() == Keys::K)
	{
		ChunkManager::setChunksSize(ChunkManager::getChunksSize() + 1);
	}

	if (e.getKeyCode() == Keys::L)
	{
		int size = ChunkManager::getChunksSize();
		if(size > 1) ChunkManager::setChunksSize(size - 1);
	}
}

float Terrain::getYCoord(float x, float z)
{
	return noise({ x, 0.f, z}, m_octaves);
}

float Terrain::hash(float n)
{
	double x = sin(n) * 753.5453123;
	return x - floor(x);
}

double Terrain::mix(double a, double b, double weight)
{
	return a * (1 - weight) + b * weight;
}

float Terrain::snoise(glm::vec3 x)
{
	glm::vec3 p = floor(x);
	glm::vec3 f = fract(x);
	f = f * f * (3.0f - (2.0f * f));

	float n = p.x + p.y * 157.0f + 113.0f * p.z;
	return mix(mix(mix(hash(n + 0.0f), hash(n + 1.0f), f.x),
		mix(hash(n + 157.0f), hash(n + 158.0f), f.x), f.y),
		mix(mix(hash(n + 113.0f), hash(n + 114.0f), f.x),
			mix(hash(n + 270.0f), hash(n + 271.0f), f.x), f.y), f.z);
}

float Terrain::noise(glm::vec3 position, int octaves)
{
	float total = 0.0;
	float frequency = m_frequency;
	float maxAmplitude = 0.0;
	float amplitude = m_amplitude;
	float scale = m_scale;

	for (int i = 0; i < octaves; i++)
	{
		total += snoise(position * frequency) * amplitude;
		frequency *= m_frequencyMultiplier;
		amplitude /= m_amplitudeDivisor;
		maxAmplitude += amplitude;
	}
	return (total / maxAmplitude) * scale;
}