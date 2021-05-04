/*! \file water.cpp
*
* \brief A script to create a water shader
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/water.h"

#define WAVESPEED = 0.03f

Water::Water()
{
	m_moveFactor = 0.f;
	ResourceManager::getResource<UniformBuffer>("WaterUBO")->uploadData("u_moveFactor", &m_moveFactor);
}

Water::~Water()
{
}

void Water::onPostUpdate(const float timestep, const float totalTime)
{
	m_moveFactor += (0.03f * timestep);
	if (m_moveFactor > 1.f)
		m_moveFactor = 0.f;

	ResourceManager::getResource<UniformBuffer>("WaterUBO")->uploadData("u_moveFactor", &m_moveFactor);
}

void Water::onRender(const Renderers renderer, const std::string & renderState)
{
}

void Water::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
}
