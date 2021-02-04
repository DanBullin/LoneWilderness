/*! \file renderPass.cpp
*
* \brief A render pass constituting the rendering of the scene to a particular target
*
* \author Daniel Bullin
*
*/

#include "independent/rendering/renderPasses/renderPass.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/components/scene.h"

namespace Engine
{
	//! RenderPass()
	RenderPass::RenderPass()
	{
		m_attachedScene = nullptr;
	}

	//! ~RenderPass()
	RenderPass::~RenderPass()
	{
		ENGINE_INFO("[RenderPass::~RenderPass] Deleting render pass.");
		m_attachedScene = nullptr;
		m_frameBuffer = nullptr;
	}

	//! attachScene()
	/*!
	\param scene a Scene* - A pointer to the scene this render pass is attached to
	*/
	void RenderPass::attachScene(Scene* scene)
	{
		if (scene)
			m_attachedScene = scene;
		else
			ENGINE_ERROR("[RenderPass::attachScene] Scene is not a valid pointer.");
	}
}