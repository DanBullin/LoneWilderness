/*! \file renderUtils.h
*
* \brief A utility class which issues calls to the rendering API
*
* \author Daniel Bullin
*
*/
#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \enum RenderParameter
	* \brief The parameters associated with rendering API
	*/
	enum class RenderParameter
	{
		None,
		COLOR_BUFFER_BIT,
		DEPTH_BUFFER_BIT,
		STENCIL_BUFFER_BIT,
		COLOR_AND_DEPTH_BUFFER_BIT,
		COLOR_AND_DEPTH_AND_STENCIL_BUFFER_BIT,
		NEVER,
		LESS,
		EQUAL,
		LESS_THAN_OR_EQUAL,
		GREATER,
		NOTEQUAL,
		GREATER_THAN_OR_EQUAL,
		ALWAYS,
		KEEP,
		REPLACE
	};

	/*! \class RenderUtils
	* \brief A utility class which issues calls to the rendering API
	*/
	class RenderUtils
	{
	public:
		static void enableFaceCulling(const bool enable); //!< Enable/Disable face culling
		static void enableDepthTesting(const bool enable); //!< Enable/Disable depth testing
		static void enableStencilTesting(const bool enable); //!< Enable/Disable stencil testing
		static void enableWireframe(const bool enable); //!< Enable/Disable wireframe mode
		static void enableDepthWriting(const bool enable); //!< Enable/Disable depth writing
		static void enableBlending(const bool enable); //!< Enable/Disable blending
		static void enablePatchDrawing(const bool enable); //!< Enable/Disable drawing with patches
		static void enableClipDistance(const bool enable); //!< Enable clip distance rendering

		static void clearBuffers(const RenderParameter buffers, const glm::vec4& colour = glm::vec4(0.f, 0.f, 0.f, 1.f)); //!< Clear the buffers

		static void setDepthComparison(const RenderParameter comparison); //!< Set the depth buffer comparison value
		static void setStencilFunc(const RenderParameter comparison, int ref, uint32_t mask); //!< Set the stencil function
		static void setStencilOp(const RenderParameter sfail, const RenderParameter dpfail, const RenderParameter dppass); //!< Set the stencil op
		static void setStencilMask(uint32_t mask); //!< Set the stencil mask
		static void setViewport(const int x, const int y, const int width, const int height); //!< Resize the viewport

		static void draw(const uint32_t drawCount); //!< Draw the geometry
		static void drawMultiIndirect(const uint32_t commandsSize); //!< Draw the geometry
	};
}
#endif