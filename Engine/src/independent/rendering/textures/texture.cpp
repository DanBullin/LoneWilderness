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
	//! Texture()
	/*
	\param textureName a const std::string& - The name of the texture
	*/
	Texture::Texture(const std::string& textureName) : Resource(textureName, ResourceType::Texture)
	{
	}

	//! ~Texture()
	Texture::~Texture()
	{
	}

	//! Texture2D()
	/*!
	\param textureName a const std::string& - The name of the texture
	\param properties a const TextureProperties& - A reference to the properties of the 2D texture
	*/
	Texture2D::Texture2D(const std::string& textureName, const TextureProperties& properties) : Texture(textureName), m_textureProperties(properties)
	{
	}

	//! ~Texture2D()
	Texture2D::~Texture2D()
	{
	}

	//! CubeMapTexture()
	/*
	\param textureName a const std::string& - The name of the texture
	*/
	CubeMapTexture::CubeMapTexture(const std::string& textureName) : Texture(textureName)
	{
	}

	//! ~CubeMapTexture()
	CubeMapTexture::~CubeMapTexture()
	{
	}

	//! create()
	/*!
	\param textureName a const std::string& - The name of the texture
	\param properties a const TextureProperties& - A reference to the properties of the 2D texture
	\param channels a const uint32_t - The number of channels in the texture data
	\param data an unsigned char* - The data of the texture
	\return an Texture2D* - The 2D texture of type defined by the graphics API chosen
	*/
	Texture2D* Texture2D::create(const std::string& textureName, const TextureProperties& properties, const uint32_t channels, unsigned char* data)
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
				return new OpenGLTexture2D(textureName, properties, channels, data);
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
	\param textureName a const std::string& - The name of the texture
	\param filePath a const char* - The filepath of the texture
	\param properties a TextureProperties - The properties of the texture
	\return a Texture2D* - The 2D texture of type defined by the graphics API chosen
	*/
	Texture2D* Texture2D::create(const std::string& textureName, const char* filePath, TextureProperties properties)
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
				return new OpenGLTexture2D(textureName, filePath, properties);
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
	\param textureName a const std::string& - The name of the texture
	\param folderPath a const std::string& - The path to the folder containing the individual files
	\param fileType a const std::string& - The file type of the textures
	\return a CubeMapTexture* - The cubmeap texture of type defined by the graphics API chosen
	*/
	CubeMapTexture* CubeMapTexture::create(const std::string& textureName, const std::string& folderPath, const std::string& fileType)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[CubeMapTexture::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLCubeMapTexture(textureName, folderPath, fileType);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[CubeMapTexture::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[CubeMapTexture::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}