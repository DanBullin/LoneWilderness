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
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer3D.h"

//! Terrain()
Terrain::Terrain()
{
	m_chunkSize = { 50, 50 };
	m_chunkStepSize = 10;
	Chunk::createGeometry(m_chunkSize.x, m_chunkSize.y, m_chunkStepSize);

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

	m_chunk = Chunk({ 0.f, 0.f, 0.f });
}

//! ~Terrain()
Terrain::~Terrain()
{
}

//! onAttach()
void Terrain::onAttach()
{
}

void Terrain::onPostUpdate(const float timestep, const float totalTime)
{
	glm::vec3 playerPos = getParent()->getParentScene()->getEntity("Player1")->getComponent<Transform>()->getPosition();
	int chunkX = round(playerPos.x / (m_chunkSize.x * m_chunkStepSize));
	int chunkY = round(playerPos.z / (m_chunkSize.y * m_chunkStepSize));
	ENGINE_INFO("{0}, {1}", chunkX, chunkY);
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
		m_chunk.onRender(renderer, renderState);
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
		m_amplitude += 20.f;
	}

	if (e.getKeyCode() == Keys::L)
	{
		m_amplitude -= 20.f;
	}
}