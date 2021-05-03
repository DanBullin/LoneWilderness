/*! \file renderUtils.cpp
*
* \brief A utility class which issues calls to the rendering API
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderUtils.h"
#include "independent/rendering/renderAPI.h"
#include "independent/systems/systems/log.h"
#include "platform/OpenGL/openGLRenderUtils.h"

namespace Engine
{
	//! enableFaceCulling()
	/*!
	\param enable a const bool - Enable face culling
	*/
	void RenderUtils::enableFaceCulling(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enableFaceCulling] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enableFaceCulling(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enableFaceCulling] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enableFaceCulling] Vulkan not supported.");
			break;
		}
		}
	}

	//! enableDepthTesting()
	/*!
	\param enable a bool - Enable depth testing
	*/
	void RenderUtils::enableDepthTesting(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enableDepthTesting] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enableDepthTesting(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enableDepthTesting] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enableDepthTesting] Vulkan not supported.");
			break;
		}
		}
	}

	//! enableStencilTesting()
	/*!
	\param enable a bool - Enable stencil testing
	*/
	void RenderUtils::enableStencilTesting(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enableStencilTesting] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enableStencilTesting(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enableStencilTesting] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enableStencilTesting] Vulkan not supported.");
			break;
		}
		}
	}

	//! enableWireframe()
	/*!
	\param enable a bool - Enable wire frame
	*/
	void RenderUtils::enableWireframe(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enableWireframe] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enableWireframe(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enableWireframe] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enableWireframe] Vulkan not supported.");
			break;
		}
		}
	}

	//! enableDepthWriting()
	/*!
	\param enable a bool - Enable depth writing
	*/
	void RenderUtils::enableDepthWriting(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enableDepthWriting] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enableDepthWriting(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enableDepthWriting] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enableDepthWriting] Vulkan not supported.");
			break;
		}
		}
	}

	//! enableBlending()
	/*!
	\param enable a bool - Enable blending
	*/
	void RenderUtils::enableBlending(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enableBlending] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enableBlending(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enableBlending] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enableBlending] Vulkan not supported.");
			break;
		}
		}
	}

	//! enablePatchDrawing()
	/*!
	\param enable a bool - Enable patch drawing
	*/
	void RenderUtils::enablePatchDrawing(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enablePatchDrawing] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enablePatchDrawing(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enablePatchDrawing] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enablePatchDrawing] Vulkan not supported.");
			break;
		}
		}
	}

	void RenderUtils::enableClipDistance(const bool enable)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::enableClipDistance] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::enableClipDistance(enable);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::enableClipDistance] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::enableClipDistance] Vulkan not supported.");
			break;
		}
		}
	}

	//! clearBuffers()
	/*!
	\param buffers a const RenderParameter - The buffers to clear
	\param colour a const glm::vec4& - The colour to clear the colour buffer with
	*/
	void RenderUtils::clearBuffers(const RenderParameter buffers, const glm::vec4& colour)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::clearBuffers] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::clearBuffers(buffers, colour);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::clearBuffers] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::clearBuffers] Vulkan not supported.");
			break;
		}
		}
	}

	//! setDepthComparison()
	/*!
	\param comparison a const RenderParameter - The depth test comparison value
	*/
	void RenderUtils::setDepthComparison(const RenderParameter comparison)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::setDepthComparison] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::setDepthComparison(comparison);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::setDepthComparison] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::setDepthComparison] Vulkan not supported.");
			break;
		}
		}
	}

	//! setStencilFunc()
	/*!
	\param comparison a const RenderParameter - The stencil test comparison value
	\param ref an int - The reference value for the stencil testt
	\param mask an uint32_t - The mask that is ANDed with the ref value and the stored stencil value
	*/
	void RenderUtils::setStencilFunc(const RenderParameter comparison, int ref, uint32_t mask)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::setStencilFunc] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::setStencilFunc(comparison, ref, mask);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::setStencilFunc] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::setStencilFunc] Vulkan not supported.");
			break;
		}
		}
	}

	//! setStencilOp()
	/*!
	\param sfail a const RenderParameter - The action to take when the stencil test fails
	\param dpfail a const RenderParameter - The action to take when the stencil test passes but depth fails
	\param dppass a const RenderParameter - The action to take when the stencil test passes
	*/
	void RenderUtils::setStencilOp(const RenderParameter sfail, const RenderParameter dpfail, const RenderParameter dppass)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::setStencilOp] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::setStencilOp(sfail, dpfail, dppass);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::setStencilOp] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::setStencilOp] Vulkan not supported.");
			break;
		}
		}
	}

	//! setStencilMask()
	/*
	\param mask an uint32_t - The mask
	*/
	void RenderUtils::setStencilMask(uint32_t mask)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::setStencilMask] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::setStencilMask(mask);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::setStencilMask] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::setStencilMask] Vulkan not supported.");
			break;
		}
		}
	}

	//! resizeViewport()
	/*!
	\param x a const int - The lower left corner of the viewport x coordinate
	\param y a const int - The lower left corner of the viewport y coordinate
	\param width a const int - The width of the viewport
	\param height a const int - The height of the viewport
	*/
	void RenderUtils::setViewport(const int x, const int y, const int width, const int height)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::setViewport] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::setViewport(x, y, width, height);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::setViewport] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::setViewport] Vulkan not supported.");
			break;
		}
		}
	}

	//! draw()
	/*!
	\param drawCount a const uint32_t - The number of indices
	*/
	void RenderUtils::draw(const uint32_t drawCount)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::draw] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::draw(drawCount);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::draw] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::draw] Vulkan not supported.");
			break;
		}
		}
	}

	//! drawMultiIndirect()
	/*!
	\param commandsSize a const uint32_t - The number of commands
	*/
	void RenderUtils::drawMultiIndirect(const uint32_t commandsSize)
	{
		switch (RenderAPI::getAPI())
		{
		case GraphicsAPI::None:
		{
			ENGINE_ERROR("[RenderUtils::drawMultiIndirect] No rendering API selected.");
			break;
		}
		case GraphicsAPI::OpenGL:
		{
			OpenGLRenderUtils::drawMultiIndirect(commandsSize);
			break;
		}
		case GraphicsAPI::Direct3D:
		{
			ENGINE_ERROR("[RenderUtils::drawMultiIndirect] Direct3D not supported.");
			break;
		}
		case GraphicsAPI::Vulkan:
		{
			ENGINE_ERROR("[RenderUtils::drawMultiIndirect] Vulkan not supported.");
			break;
		}
		}
	}
}