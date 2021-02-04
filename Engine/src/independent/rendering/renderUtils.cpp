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

	//! clearBuffers()
	/*!
	\param buffers a const RenderParameters::RenderParameter - The buffers to clear
	\param colour a const glm::vec4& - The colour to clear the colour buffer with
	*/
	void RenderUtils::clearBuffers(const RenderParameters::RenderParameter buffers, const glm::vec4& colour)
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
	\param comparison a const RenderParameters::RenderParameter - The depth test comparison value
	*/
	void RenderUtils::setDepthComparison(const RenderParameters::RenderParameter comparison)
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