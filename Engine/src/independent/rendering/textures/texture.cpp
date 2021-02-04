/*! \file texture.cpp
*
* \brief An API agnostic texture
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/log.h"
#include "independent/rendering/renderAPI.h"
#include "independent/rendering/textures/texture.h"
#include "platform/OpenGL/textures/openGLTexture.h"

namespace Engine
{
	//! create()
	/*!
	\param properties a const Texture2DProperties& - A reference to the properties of the 2D texture
	\param channels a const uint32_t - The number of channels in the texture data
	\param data an unsigned char* - The data of the texture
	\return an Texture2D* - The 2D texture of type defined by the graphics API chosen
	*/
	Texture2D* Texture2D::create(const Texture2DProperties& properties, const uint32_t channels, unsigned char* data)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[Texture2D::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLTexture2D(properties, channels, data);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[Texture2D::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[Texture2D::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}

	//! create()
	/*!
	\param filePath a const char* - The filepath of the texture
	\param properties a Texture2DProperties - A reference to the properties of the texture
	\return an Texture2D* - The 2D texture of type defined by the graphics API chosen
	*/
	Texture2D* Texture2D::create(const char* filePath, Engine::Texture2DProperties properties)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[Texture2D::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLTexture2D(filePath, properties);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[Texture2D::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[Texture2D::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}