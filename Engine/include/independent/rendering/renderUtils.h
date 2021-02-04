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
	namespace RenderParameters
	{
		/*! \enum RenderParameter
		* \brief The parameters associated with rendering API
		*/
		enum RenderParameter
		{
			None = 0,
			COLOR_BUFFER_BIT = 1,
			DEPTH_BUFFER_BIT = 2,
			COLOR_AND_DEPTH_BUFFER_BIT = 3,
			NEVER = 4,
			LESS = 5,
			EQUAL = 6,
			LESS_THAN_OR_EQUAL = 7,
			GREATER = 8,
			NOTEQUAL = 9,
			GREATER_THAN_OR_EQUAL = 10,
			ALWAYS = 11
		};
	}

	/*! \class RenderUtils
	* \brief A utility class which issues calls to the rendering API
	*/
	class RenderUtils
	{
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