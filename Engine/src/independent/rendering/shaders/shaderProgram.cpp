/*! \file shaderProgram.cpp
*
* \brief An API agnostic shader program
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/log.h"
#include "independent/rendering/renderAPI.h"
#include "independent/rendering/shaders/shaderProgram.h"
#include "platform/OpenGL/shaders/openGLShaderProgram.h"

namespace Engine
{
	//! create()
	/*!
	\return a ShaderProgram* - The shader object of type defined by the graphics API chosen
	*/
	ShaderProgram* ShaderProgram::create()
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[ShaderProgram::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLShaderProgram;
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[ShaderProgram::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[ShaderProgram::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}