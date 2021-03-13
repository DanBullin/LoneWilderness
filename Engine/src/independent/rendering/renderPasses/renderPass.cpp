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
	bool RenderPass::s_initialised = false; //!< Initialise to false

	//! RenderPass()
	RenderPass::RenderPass()
	{
		m_attachedScene = nullptr;
		m_index = 0;
		m_enabled = true;

		if (!s_initialised)
		{
			// Create the Text related resources (SubTextures, Shader and Material)
			SubTexture* textSubTexture = new SubTexture("textSubTexture", ResourceManager::getResource<Texture2D>("defaultTexture"), { 0.f, 0.f }, { 1.f, 1.f }, false);
			ResourceManager::registerResource("textSubTexture", textSubTexture);

			ShaderProgram* newShader = ShaderProgram::create("textShader");
			newShader->build(ResourceManager::getResource<VertexArray>("QuadArray"), "assets/shaders/quad/vertex.vs", "assets/shaders/quad/fragment.fs", "", "", "");
			newShader->setUniforms({ "u_textures" });
			newShader->setUniformBuffers({ { "Camera", ResourceManager::getResource<UniformBuffer>("CameraUBO") } });
			newShader->setOrderImportance(2);
			ResourceManager::registerResource("textShader", newShader);

			Material* textMaterial = new Material("textMaterial", { textSubTexture }, {}, ResourceManager::getResource<ShaderProgram>("textShader"), { 1.f, 1.f, 1.f, 1.f }, 32.f);
			ResourceManager::registerResource("textMaterial", textMaterial);

			// Create the screen quad subtextures
			SubTexture* newSubTexture1 = new SubTexture("screenQuadSubTexture1", ResourceManager::getResource<Texture2D>("defaultTexture"), { 0.f, 0.f }, { 1.f, 1.f }, true);
			SubTexture* newSubTexture2 = new SubTexture("screenQuadSubTexture2", ResourceManager::getResource<Texture2D>("defaultTexture"), { 0.f, 0.f }, { 1.f, 1.f }, true);
			ResourceManager::registerResource("screenQuadSubTexture1", newSubTexture1);
			ResourceManager::registerResource("screenQuadSubTexture2", newSubTexture2);

			s_initialised = true;
		}
	}

	//! ~RenderPass()
	RenderPass::~RenderPass()
	{
		ENGINE_INFO("[RenderPass::~RenderPass] Deleting render pass.");
		m_attachedScene = nullptr;
		s_initialised = false;
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