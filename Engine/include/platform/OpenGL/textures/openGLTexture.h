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
		OpenGLTexture2D(const char* filePath, Texture2DProperties& properties); //!< Constructor
		OpenGLTexture2D(const Texture2DProperties& properties, const uint32_t channels, unsigned char* data); //!< Constructor
		~OpenGLTexture2D(); //!< Destructor
		virtual void edit(const uint32_t offsetX, const uint32_t offsetY, const uint32_t width, const uint32_t height, const unsigned char* data) override; //!< Edit the texture data
		virtual void bind(const uint32_t slot = 0) override; //!< Bind the texture to a texture unit
	};
}
#endif