/*! \file openGLRenderUtils.cpp
*
* \brief A utility class which issues calls to OpenGL
*
* \author Daniel Bullin
*
*/
#include "platform/OpenGL/openGLRenderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/log.h"
#include <glad/glad.h>

namespace Engine
{
	bool OpenGLRenderUtils::s_depthTesting = false; //!< Is depth testing enabled
	bool OpenGLRenderUtils::s_faceCulling = false; //!< Is face culling enabled
	bool OpenGLRenderUtils::s_depthWriting = true; //!< Is depth writing enabled
	bool OpenGLRenderUtils::s_wireframeMode = false; //!< Is wireframe mode enabled
	bool OpenGLRenderUtils::s_blending = false; //!< Is blending enabled
	bool OpenGLRenderUtils::s_patchDrawing = false; //!< Is patch drawing enabled

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
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages)) 
				ENGINE_TRACE("[OpenGLRenderUtils::enableFaceCulling] Enabled face culling.");
		}
		else if (!enable && s_depthTesting)
		{
			glDisable(GL_CULL_FACE);
			s_faceCulling = false;
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableFaceCulling] Disabled face culling.");
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
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableDepthTesting] Enabled depth testing.");
		}
		else if (!enable && s_depthTesting)
		{
			glDisable(GL_DEPTH_TEST);
			s_depthTesting = false;
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableDepthTesting] Disabled depth testing.");
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
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableWireframe] Enabled wireframe mode.");
		}
		else if (!enable && s_wireframeMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			s_wireframeMode = false;
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableWireframe] Disabled wireframe mode.");
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
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableDepthWriting] Enabled depth writing.");
		}
		else if (!enable && s_depthTesting)
		{
			glDepthMask(GL_FALSE);
			s_depthWriting = false;
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableDepthWriting] Disabled depth writing.");
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
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableBlending] Enabled blending.");
		}
		else if (!enable && s_blending)
		{
			glDisable(GL_BLEND);
			s_blending = false;
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enableBlending] Disabled blending.");
		}
	}

	//! enablePatchDrawing()
	/*!
	\param enable a bool - Enable patch drawing
	*/
	void OpenGLRenderUtils::enablePatchDrawing(const bool enable)
	{
		if (enable && !s_patchDrawing)
		{
			s_patchDrawing = true;
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enablePatchDrawing] Enabled Patch drawing.");
		}
		else if (!enable && s_patchDrawing)
		{
			s_patchDrawing = false;
			if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
				ENGINE_TRACE("[OpenGLRenderUtils::enablePatchDrawing] Disabled Patch drawing.");
		}
	}

	//! clearBuffers()
	/*!
	\param buffers a const RenderParameter - The buffers to clear
	\param colour a const glm::vec4& - The colour to clear the colour buffer with
	*/
	void OpenGLRenderUtils::clearBuffers(const RenderParameter buffers, const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
		glClear(RenderParameters::toGLType(buffers));
		if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
			ENGINE_TRACE("[OpenGLRenderUtils::clearBuffers] Clearing buffers to: {0}, {1}, {2}, {3}.", colour.r, colour.g, colour.b, colour.a);
	}

	//! setDepthComparison()
	/*!
	\param comparison a const RenderParameter - The depth test comparison value
	*/
	void OpenGLRenderUtils::setDepthComparison(const RenderParameter comparison)
	{
		glDepthFunc(RenderParameters::toGLType(comparison));
		if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
			ENGINE_TRACE("[OpenGLRenderUtils::setDepthComparison] Setting depth comparison to {0}.", static_cast<uint32_t>(comparison));
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
		if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
			ENGINE_TRACE("[OpenGLRenderUtils::setViewport] Resizing viewport. X: {0}, Y: {1}, Width: {2}, Height: {3}.", x, y, width, height);
	}

	//! draw()
	/*!
	\param drawCount a const uint32_t - The number of indices
	*/
	void OpenGLRenderUtils::draw(const uint32_t drawCount)
	{
		if (s_patchDrawing)
			glDrawElements(GL_PATCHES, drawCount, GL_UNSIGNED_INT, nullptr);
		else
			glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, nullptr);

		if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
			ENGINE_TRACE("[OpenGLRenderUtils::draw] Drawing elements. Count: {0}.", drawCount);
	}

	//! drawMultiIndirect()
	/*!
	\param commandsSize a const uint32_t - The number of commands
	*/
	void OpenGLRenderUtils::drawMultiIndirect(const uint32_t commandsSize)
	{
		if(s_patchDrawing)
			glMultiDrawElementsIndirect(GL_PATCHES, GL_UNSIGNED_INT, (GLvoid*)0, commandsSize, 0);
		else
			glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, commandsSize, 0);

		if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages))
			ENGINE_TRACE("[OpenGLRenderUtils::drawMultiIndirect] Drawing multi elements. Count: {0}.", commandsSize);
	}
}