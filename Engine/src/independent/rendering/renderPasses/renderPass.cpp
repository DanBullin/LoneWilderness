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
		m_index = 0;
		m_enabled = true;
	}

	//! ~RenderPass()
	RenderPass::~RenderPass()
	{
		ENGINE_INFO("[RenderPass::~RenderPass] Deleting render pass.");
		m_attachedScene = nullptr;
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

	//! setEnabled()
	/*!
	\param value a const bool - Set whether this render pass is enabled
	*/
	void RenderPass::setEnabled(const bool value)
	{
		m_enabled = value;
	}

	//! getEnabled()
	/*!
	\return a bool - Is this render pass enabled
	*/
	bool RenderPass::getEnabled() const
	{
		return m_enabled;
	}

	//! setIndex()
	/*
	\param index a const uint32_t - The index of the pass
	*/
	void RenderPass::setIndex(const uint32_t index)
	{
		m_index = index;
	}

	//! getIndex()
	/*
	\return an uint32_t - The index of the pass in the scene's list of passes
	*/
	uint32_t RenderPass::getIndex() const
	{
		return m_index;
	}
}