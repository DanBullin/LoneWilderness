/*! \file openGLRenderUtils.h
*
* \brief A utility class which issues calls to OpenGL
*
* \author Daniel Bullin
*
*/
#ifndef OPENGLRENDERUTILS_H
#define OPENGLRENDERUTILS_H

#include "independent/rendering/renderUtils.h"

namespace Engine
{
	/*! \class OpenGLRenderUtils
	* \brief A utility class which issues calls to OpenGL
	*/
	class OpenGLRenderUtils
	{
	private:
		static bool s_depthTesting; //!< Should depth testing be enabled
		static bool s_stencilTesting; //!< Should stencil testing be enabled
		static bool s_faceCulling; //!< Should face culling be enabled
		static bool s_depthWriting; //!< The depth mask which allows or disallows writing to the depth buffer
		static bool s_wireframeMode; //!< Should we draw geometry in wireframe mode
		static bool s_blending; //!< Should blending be enabled
		static bool s_patchDrawing; //!< Should drawing primitive be patches
		static bool s_clipDistance; //!< Should clip distance be enabled
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