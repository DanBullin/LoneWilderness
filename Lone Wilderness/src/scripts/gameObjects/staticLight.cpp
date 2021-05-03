/*! \file staticLight.cpp
*
* \brief A script for a static light in the scene
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/staticLight.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer3D.h"

//! StaticLight()
StaticLight::StaticLight()
{
}

//! ~StaticLight()
StaticLight::~StaticLight()
{
}

//! onSubmit()
/*!
\param renderer a const Renderers - The renderer being called
\param renderState a const std::string& - The rendering state
*/
void StaticLight::onSubmit(const Renderers renderer, const std::string& renderState)
{
	if (getParent()->containsComponent<MeshRender3D>())
	{
		getParent()->getComponent<MeshRender3D>()->getMaterial()->setTint({ getParent()->getComponent<PointLight>()->getDiffuseFactor(), 1.f });
	}
}