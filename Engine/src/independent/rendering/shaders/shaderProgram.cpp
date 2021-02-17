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
	//! ShaderProgram()
	/*
	\param shaderName a const std::string& - The name of the shader program
	*/
	ShaderProgram::ShaderProgram(const std::string& shaderName) : Resource(shaderName, ResourceType::ShaderProgram), m_vertexArray(nullptr)
	{
	}

	//! ~ShaderProgram()
	ShaderProgram::~ShaderProgram()
	{
	}

	//! create()
	/*!
	\param shaderName a const std::string& - The name of the shader program
	\return a ShaderProgram* - The shader object of type defined by the graphics API chosen
	*/
	ShaderProgram* ShaderProgram::create(const std::string& shaderName)
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
				return new OpenGLShaderProgram(shaderName);
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