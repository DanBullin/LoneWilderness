/*! \file OpenGLTexture.h
*
* \brief An OpenGL Texture
*
* \author DMU Course material
*
*/
#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include "independent/rendering/textures/texture.h"

namespace Engine
{
	/*! \class OpenGLTexture2D
	* \brief An OpenGL texture 2D object
	*/
	class OpenGLTexture2D : public Texture2D
	{
	private:
		void init(unsigned char* data, const uint32_t channels); //!< Create the texture
	public:
		OpenGLTexture2D(const std::string& textureName, const char* filePath, TextureProperties& properties); //!< Constructor
		OpenGLTexture2D(const std::string& textureName, const TextureProperties& properties, const uint32_t channels, unsigned char* data); //!< Constructor
		~OpenGLTexture2D(); //!< Destructor
		void edit(const uint32_t offsetX, const uint32_t offsetY, const uint32_t width, const uint32_t height, const unsigned char* data) override; //!< Edit the texture data
		void bind(const uint32_t slot = 0) override; //!< Bind the texture to a texture unit
		void printDetails() override; //!< Print the resource details
	};

	/*! \class OpenGLCubeMapTexture
	* \brief An OpenGL cubemap texture object
	*/
	class OpenGLCubeMapTexture : public CubeMapTexture
	{
	public:
		OpenGLCubeMapTexture(const std::string& textureName, const std::string& folderPath, const std::string& fileType); //!< Constructor
		~OpenGLCubeMapTexture(); //!< Destructor

		void bind(const uint32_t slot = 0) override; //!< Bind the texture to a texture unit
		void printDetails() override; //!< Print the resource details
	};
}
#endif