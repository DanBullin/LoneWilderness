/*! \file openGLRenderUtils.h
*
* \brief A utility class which issues calls to OpenGL
*
* \author Daniel Bullin
*
*/
#ifndef OPENGLRENDERUTILS_H
#define OPENGLRENDERUTILS_H

#include "independent/core/common.h"
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
		static bool s_faceCulling; //!< Should face culling be enabled
		static bool s_depthWriting; //!< The depth mask which allows or disallows writing to the depth buffer
		static bool s_wireframeMode; //!< Should we draw geometry in wireframe mode
		static bool s_blending; //!< Should blending be enabled
	public:
		static void enableFaceCulling(const bool enable); //!< Enable/Disable face culling
		static void enableDepthTesting(const bool enable); //!< Enable/Disable depth testing
		static void enableWireframe(const bool enable); //!< Enable/Disable wireframe mode
		static void enableDepthWriting(const bool enable); //!< Enable/Disable depth writing
		static void enableBlending(const bool enable); //!< Enable/Disable blending

		static void clearBuffers(const RenderParameters::RenderParameter buffers, const glm::vec4& colour = glm::vec4(0.f, 0.f, 0.f, 1.f)); //!< Clear the buffers

		static void setDepthComparison(const RenderParameters::RenderParameter comparison); //!< Set the depth buffer comparison value
		static void setViewport(const int x, const int y, const int width, const int height); //!< Resize the viewport

		static void draw(const uint32_t drawCount); //!< Draw the geometry
		static void drawMultiIndirect(const uint32_t commandsSize); //!< Draw the geometry
	};
}
#endif