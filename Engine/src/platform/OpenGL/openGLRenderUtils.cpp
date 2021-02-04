/*! \file openGLRenderUtils.cpp
*
* \brief A utility class which issues calls to OpenGL
*
* \author Daniel Bullin
*
*/

#include "platform/OpenGL/openGLRenderUtils.h"

#include <glad/glad.h>

namespace Engine
{
	bool OpenGLRenderUtils::s_depthTesting = false; //!< Is depth testing enabled
	bool OpenGLRenderUtils::s_faceCulling = false; //!< Is face culling enabled
	bool OpenGLRenderUtils::s_depthWriting = true; //!< Is depth writing enabled
	bool OpenGLRenderUtils::s_wireframeMode = false; //!< Is wireframe mode enabled
	bool OpenGLRenderUtils::s_blending = false; //!< Is blending enabled

	namespace RenderParameters
	{
		//! toGLType()
		/*!
		\param type a const RenderParameter - The parameter type
		\return an uint32_t - The OpenGL parameter value
		*/
		static uint32_t toGLType(const RenderParameter type)
		{
			switch (type)
			{
			case RenderParameter::COLOR_BUFFER_BIT: return GL_COLOR_BUFFER_BIT;
			case RenderParameter::DEPTH_BUFFER_BIT: return GL_DEPTH_BUFFER_BIT;
			case RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT: return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			case RenderParameter::NEVER: return GL_NEVER;
			case RenderParameter::LESS: return GL_LESS;
			case RenderParameter::EQUAL: return GL_EQUAL;
			case RenderParameter::LESS_THAN_OR_EQUAL: return GL_LEQUAL;
			case RenderParameter::GREATER: return GL_GREATER;
			case RenderParameter::NOTEQUAL: return GL_NOTEQUAL;
			case RenderParameter::GREATER_THAN_OR_EQUAL: return GL_GEQUAL;
			case RenderParameter::ALWAYS: return GL_ALWAYS;
			default: return GL_INVALID_ENUM;
			}
		}
	}

	//! enableFaceCulling()
	/*!
	\param enable a const bool - Enable face culling
	*/
	void OpenGLRenderUtils::enableFaceCulling(const bool enable)
	{
		// Check if the value passed is the currently set value
		// Change if so, otherwise settings already set
		if (enable && !s_faceCulling)
		{
			glEnable(GL_CULL_FACE);
			s_faceCulling = true;
		}
		else if (!enable && s_depthTesting)
		{
			glDisable(GL_CULL_FACE);
			s_faceCulling = false;
		}
	}

	//! enableDepthTesting()
	/*!
	\param enable a bool - Enable depth testing
	*/
	void OpenGLRenderUtils::enableDepthTesting(const bool enable)
	{
		// Check if the value passed is the currently set value
		// Change if so, otherwise settings already set
		if (enable && !s_depthTesting)
		{
			glEnable(GL_DEPTH_TEST);
			s_depthTesting = true;
		}
		else if (!enable && s_depthTesting)
		{
			glDisable(GL_DEPTH_TEST);
			s_depthTesting = false;
		}
	}

	//! enableWireframe()
	/*!
	\param enable a bool - Enable wire frame
	*/
	void OpenGLRenderUtils::enableWireframe(const bool enable)
	{
		// Check if the value passed is the currently set value
		// Change if so, otherwise settings already set
		if (enable && !s_wireframeMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			s_wireframeMode = true;
		}
		else if (!enable && s_wireframeMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			s_wireframeMode = false;
		}
	}

	//! enableDepthWriting()
	/*!
	\param enable a bool - Enable depth writing
	*/
	void OpenGLRenderUtils::enableDepthWriting(const bool enable)
	{
		// Check if the value passed is the currently set value
		// Change if so, otherwise settings already set
		if (enable && !s_depthWriting)
		{
			glDepthMask(GL_TRUE);
			s_depthWriting = true;
		}
		else if (!enable && s_depthTesting)
		{
			glDepthMask(GL_FALSE);
			s_depthWriting = false;
		}
	}

	//! enableBlending()
	/*!
	\param enable a bool - Enable blending
	*/
	void OpenGLRenderUtils::enableBlending(const bool enable)
	{
		if (enable && !s_blending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			s_blending = true;
		}
		else if (!enable && s_blending)
		{
			glDisable(GL_BLEND);
			s_blending = false;
		}
	}

	//! clearBuffers()
	/*!
	\param buffers a const RenderParameters::RenderParameter - The buffers to clear
	\param colour a const glm::vec4& - The colour to clear the colour buffer with
	*/
	void OpenGLRenderUtils::clearBuffers(const RenderParameters::RenderParameter buffers, const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
		glClear(RenderParameters::toGLType(buffers));
	}

	//! setDepthComparison()
	/*!
	\param comparison a const RenderParameters::RenderParameter - The depth test comparison value
	*/
	void OpenGLRenderUtils::setDepthComparison(const RenderParameters::RenderParameter comparison)
	{
		glDepthFunc(RenderParameters::toGLType(comparison));
	}

	//! resizeViewport()
	/*!
	\param x a const int - The lower left corner of the viewport
	\param y a const int - The lower left corner of the viewport
	\param width a const int - The width of the viewport
	\param height a const int - The height of the viewport
	*/
	void OpenGLRenderUtils::setViewport(const int x, const int y, const int width, const int height)
	{
		glViewport(x, y, width, height);
	}

	//! draw()
	/*!
	\param drawCount a const uint32_t - The number of indices
	*/
	void OpenGLRenderUtils::draw(const uint32_t drawCount)
	{
		glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, nullptr);
	}

	//! drawMultiIndirect()
	/*!
	\param commandsSize a const uint32_t - The number of commands
	*/
	void OpenGLRenderUtils::drawMultiIndirect(const uint32_t commandsSize)
	{
		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, commandsSize, 0);
	}
}