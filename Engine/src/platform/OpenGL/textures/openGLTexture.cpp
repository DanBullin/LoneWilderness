/*! \file OpenGLTexture.cpp
*
* \brief An OpenGL Texture
*
* \author DMU Course material
*
*/
#include <glad/glad.h>
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "platform/OpenGL/textures/openGLTexture.h"
#include <stb_image.h>

namespace Engine
{
	//! toGLType()
	/*!
	\param type a const TextureParameter - The parameter type
	\return an uint32_t - The OpenGL parameter value
	*/
	static uint32_t toGLType(const TextureParameter type)
	{
		switch (type)
		{
			case TextureParameter::ClampToBorder: return GL_CLAMP_TO_BORDER;
			case TextureParameter::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case TextureParameter::Repeat: return GL_REPEAT;
			case TextureParameter::MirroredRepeat: return GL_MIRRORED_REPEAT;
			case TextureParameter::Nearest: return GL_NEAREST;
			case TextureParameter::Linear: return GL_LINEAR;
			case TextureParameter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
			case TextureParameter::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
			case TextureParameter::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
			case TextureParameter::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
			default: return GL_INVALID_ENUM;
		}
	}

	//! toString()
	/*!
	\param texParam a const uint32_t - The texture parameter's integer value
	\return a std::string - The string literal of the texture parameter
	*/
	static std::string toString(const uint32_t texParam)
	{
		switch (texParam)
		{
			case GL_FLOAT:
				return "Float";
			case GL_UNSIGNED_BYTE:
				return "Unsigned Byte";
			case GL_RGBA:
				return "RGBA";
			case GL_RGB:
				return "RGB";
			case GL_RED:
				return "R";
			case GL_RGBA16F:
				return "RGBA16F";
			case GL_DEPTH_COMPONENT:
				return "Depth";
			default:
				return "None";
		}
	}

	//! init()
	/*!
	\param data an unsigned char* - A pointer to the data to store
	\param channels a const uint32_t - The number of channels of the data format
	*/
	void OpenGLTexture2D::init(unsigned char* data, const uint32_t channels)
	{
		// Generate the texture
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// Set texture properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLType(m_textureProperties.WrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLType(m_textureProperties.WrapT));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, toGLType(m_textureProperties.WrapR));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLType(m_textureProperties.MinFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLType(m_textureProperties.MaxFilter));

		// Set the data format, based on the number of channels loaded and whether the image is
		// gamma corrected
		GLenum internalFormat;
		GLenum dataFormat;
		GLenum type;
		if (channels == 1)
		{
			internalFormat = dataFormat = GL_RED;
			type = GL_UNSIGNED_BYTE;
		}
		else if (channels == 2)
		{
			internalFormat = dataFormat = GL_DEPTH_COMPONENT;
			type = GL_FLOAT;
		}
		else if (channels == 3)
		{
			internalFormat = m_textureProperties.GammaCorrect ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
			type = GL_UNSIGNED_BYTE;
		}
		else if (channels == 4)
		{
			internalFormat = m_textureProperties.GammaCorrect ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
		}
		else if (channels == 5)
		{
			internalFormat = GL_RGBA16F;
			dataFormat = GL_RGBA;
			type = GL_FLOAT;
		}

		m_pixelDataType = type;
		m_internalFormat = internalFormat;

		// Load data to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_textureProperties.Width, m_textureProperties.Height, 0, dataFormat, type, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Store texture properties
		m_channels = channels;
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//! OpenGLTexture2D()
	/*!
	\param textureName a const std::string& - The name of the texture
	\param filePath a const char* - The filepath of the texture
	\param properties a TextureProperties& - A reference to the texture properties
	*/
	OpenGLTexture2D::OpenGLTexture2D(const std::string& textureName, const char* filePath, TextureProperties& properties)
		: Texture2D(textureName, properties)
	{
		int width, height, channels;
		m_pixelDataType = 0;
		m_internalFormat = 0;

		// Should we flip the UVs?
		if (m_textureProperties.FlipUVs)
			stbi_set_flip_vertically_on_load(true);
		else
			stbi_set_flip_vertically_on_load(false);

		unsigned char *data = stbi_load(filePath, &width, &height, &channels, 0);

		m_textureProperties.Width = width;
		m_textureProperties.Height = height;

		// If data is valid, then initiate the texture
		if (data) { init(data, channels); }
		else { ENGINE_ERROR("[OpenGLTexture2D::OpenGLTexture2D] Could not load the texture from file: {0}", filePath); }

		stbi_image_free(data);
	}

	//! OpenGLTexture2D()
	/*!
	\param textureName a const std::string& - The name of the texture
	\param properties a const TextureProperties& - A reference to the texture properties
	\param channels a const uint32_t - The number of channels of the data format
	\param data an unsigned char* - A pointer to the data to store
	*/
	OpenGLTexture2D::OpenGLTexture2D(const std::string& textureName, const TextureProperties& properties, const uint32_t channels, unsigned char* data)
		: Texture2D(textureName, properties)
	{
		m_pixelDataType = 0;
		m_internalFormat = 0;
		init(data, channels);
	}

	//! ~OpenGLTexture2D()
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLTexture2D::~OpenGLTexture2D] Deleting texture with ID: {0}, Name: {1}.", m_textureID, m_name);
		glDeleteTextures(1, &m_textureID);
	}

	//! edit()
	/*!
	\param offsetX a const uint32_t - The x offset in memory to edit
	\param offsetY a const uint32_t - The y offset in memory to edit
	\param width a const uint32_t - The width of the texture
	\param height a const uint32_t - The height of the texture
	\param data a const unsigned char* - A pointer to the data to store
	*/
	void OpenGLTexture2D::edit(const uint32_t offsetX, const uint32_t offsetY, const uint32_t width, const uint32_t height, const unsigned char* data)
	{
		if (data)
		{
			if (m_channels == 1)
				glTextureSubImage2D(m_textureID, 0, offsetX, offsetY, width, height, GL_RED, GL_UNSIGNED_BYTE, data);
			else if (m_channels == 3)
				glTextureSubImage2D(m_textureID, 0, offsetX, offsetY, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (m_channels == 4)
				glTextureSubImage2D(m_textureID, 0, offsetX, offsetY, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		if (ResourceManager::getConfigValue(Config::PrintOpenGLDebugMessages)) 
			ENGINE_TRACE("OpenGLTexture2D::edit] Editing texture {0} from offset: {1}, {2} with size of: {3}, {4}.", m_name, offsetX, offsetY, width, height);
	}

	//! bind()
	/*!
	\param slot a const uint32_t - Bind the texture to texture unit
	*/
	void OpenGLTexture2D::bind(const uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	//! printDetails()
	void OpenGLTexture2D::printDetails()
	{
		ENGINE_TRACE("Texture ID: {0}.", m_textureID);
		ENGINE_TRACE("Number of Channels: {0}.", m_channels);
		ENGINE_TRACE("Internal Format: {0}.", toString(m_internalFormat));
		ENGINE_TRACE("Pixel Data type: {0}.", toString(m_pixelDataType));
		ENGINE_TRACE("Number of Channels: {0}.", m_channels);
		ENGINE_TRACE("Width: {0}.", m_textureProperties.Width);
		ENGINE_TRACE("Height: {0}.", m_textureProperties.Height);
		ENGINE_TRACE("WrapS: {0}.", toString(m_textureProperties.WrapS));
		ENGINE_TRACE("WrapT: {0}.", toString(m_textureProperties.WrapT));
		ENGINE_TRACE("WrapR: {0}.", toString(m_textureProperties.WrapR));
		ENGINE_TRACE("MinFilter: {0}.", toString(m_textureProperties.MinFilter));
		ENGINE_TRACE("MaxFilter: {0}.", toString(m_textureProperties.MaxFilter));
		ENGINE_TRACE("Gamma Correct: {0}.", m_textureProperties.GammaCorrect);
		ENGINE_TRACE("Flip UVs: {0}.", m_textureProperties.FlipUVs);
	}

	//! OpenGLCubeMapTexture()
	/*!
	\param textureName a const std::string& - The name of the texture
	\param folderPath a const std::string& - The path to the folder containing the individual files
	\param fileType a const std::string& - The file type of the textures
	*/
	OpenGLCubeMapTexture::OpenGLCubeMapTexture(const std::string& textureName, const std::string& folderPath, const std::string& fileType) : CubeMapTexture(textureName)
	{
		stbi_set_flip_vertically_on_load(false);

		// Directory path to the last folder is provided, add file names and type
		std::vector<std::string> faces = { "right" + fileType, "left" + fileType, "top" + fileType, "bottom" + fileType, "front" + fileType,
										   "back" + fileType };

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

		int width, height, channels;
		m_folderPath = folderPath;

		// Loop through each face for the cubemap
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			std::string file = folderPath + faces[i];
			// Load texture image from file
			unsigned char *data = stbi_load(file.c_str(), &width, &height, &channels, 0);

			// If data is valid, then create the texture
			if (data)
			{
				if (channels == 3) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				else if (channels == 4) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				else return;
			}
			else
			{
				ENGINE_ERROR("[OpenGLCubeMapTexture::OpenGLCubeMapTexture] Could not load the texture from folder: {0}", folderPath);
				glDeleteTextures(1, &m_textureID);
			}
			stbi_image_free(data);
		}

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	//! ~OpenGLCubeMapTexture()
	OpenGLCubeMapTexture::~OpenGLCubeMapTexture()
	{
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLCubeMapTexture::~OpenGLCubeMapTexture] Deleting texture with ID: {0}, Name: {1}.", m_textureID, m_name);
		glDeleteTextures(1, &m_textureID);
	}

	//! bind()
	/*!
	\param slot a const uint32_t - Bind the texture to texture unit
	*/
	void OpenGLCubeMapTexture::bind(const uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	}

	//! printDetails()
	void OpenGLCubeMapTexture::printDetails()
	{
		ENGINE_TRACE("Texture ID: {0}.", m_textureID);
		ENGINE_TRACE("Folder Path: {0}.", m_folderPath);
	}
}