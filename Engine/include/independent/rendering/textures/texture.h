/*! \file texture.h
*
* \brief An API agnostic texture
*
* \author Daniel Bullin
*
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "independent/core/common.h"

namespace Engine
{
	namespace TextureParameters
	{
		/*! \enum TextureParameter
		* \brief The parameters associated with textures
		*/
		enum TextureParameter
		{
			None = 0,
			ClampToBorder = 1,
			ClampToEdge = 2,
			Repeat = 3,
			MirroredRepeat = 4,
			Nearest = 5,
			Linear = 6,
			NearestMipmapNearest = 7,
			LinearMipmapNearest = 8,
			NearestMipmapLinear = 9,
			LinearMipmapLinear = 10
		};

		//! toParameterType()
		/*!
		\param texParam a const std::string& - The texture parameter (Wrapping, Min filter, max filter)
		\return a TextureParameter - The texture parameter enum value
		*/
		static TextureParameter toParameterType(const std::string& texParam)
		{
			// Return the parameter type
			if (texParam == "ClampToBorder") return TextureParameter::ClampToBorder;
			if (texParam == "ClampToEdge") return TextureParameter::ClampToEdge;
			if (texParam == "Repeat") return TextureParameter::Repeat;
			if (texParam == "MirroredRepeat") return TextureParameter::MirroredRepeat;
			if (texParam == "Nearest") return TextureParameter::Nearest;
			if (texParam == "Linear") return TextureParameter::Linear;
			if (texParam == "NearestMipmapNearest") return TextureParameter::NearestMipmapNearest;
			if (texParam == "LinearMipmapNearest") return TextureParameter::LinearMipmapNearest;
			if (texParam == "NearestMipmapLinear") return TextureParameter::NearestMipmapLinear;
			if (texParam == "LinearMipmapLinear") return TextureParameter::LinearMipmapLinear;

			return TextureParameter::None;
		}
	}

	/*! \struct Texture2DProperties
	* \brief All texture 2D properties can be configured by creating an instance of this struct and passing it to the relevant create functions
	*/
	struct Texture2DProperties
	{
		uint32_t Width; //!< The width of the texture if you're not loading from file
		uint32_t Height; //!< The height of the texture if you're not loading from file
		TextureParameters::TextureParameter WrapS; //!< Action to take when wrapping around s coordinate
		TextureParameters::TextureParameter WrapT; //!< Action to take when wrapping around t coordinate
		TextureParameters::TextureParameter WrapR; //!< Action to take when wrapping around r coordinate
		TextureParameters::TextureParameter MinFilter; //!< The minifying function
		TextureParameters::TextureParameter MaxFilter; //!< The magnification function
		bool GammaCorrect; //!< Is the texture in sRGB space
		bool FlipUVs; //!< Should the UVs be flipped
		Texture2DProperties(const uint32_t width = 0, const uint32_t height = 0, const std::string& wrapS = "Repeat", const std::string& wrapT = "Repeat", const std::string& wrapR = "Repeat",
			const std::string& minFilter = "Linear", const std::string& maxFilter = "Linear", const bool gammaCorrect = false, const bool flipUVs = false) :
			Width(width), Height(height), WrapS(TextureParameters::toParameterType(wrapS)), WrapT(TextureParameters::toParameterType(wrapT)), WrapR(TextureParameters::toParameterType(wrapR)), MinFilter(TextureParameters::toParameterType(minFilter)), MaxFilter(TextureParameters::toParameterType(maxFilter)), GammaCorrect(gammaCorrect), FlipUVs(flipUVs) {} //!< Constructor
			/*!< \param width a const uint32_t - The width of the texture
				 \param height a const uint32_t - The height of the texture
				 \param wrapS a const std::string& - The wrap type along the s axis
				 \param wrapT a const std::string& - The wrap type along the t axis
				 \param wrapR a const std::string& - The wrap type along the r axis
				 \param minFilter a const std::string& - The min filter
				 \param maxFilter a const std::string& - The max filter
				 \param gammaCorrect a const bool - Do we need to gamma correct the image
				 \param flipUVs a const bool - Do we need to flip the texture UVs */
	};

	/*! \class Texture2D
	* \brief An API agnostic 2D texture
	*/
	class Texture2D
	{
	protected:
		uint32_t m_textureID; //!< Texture ID
		uint32_t m_channels; //!< The number of channels in the pixel data
		Texture2DProperties m_textureProperties; //!< The texture properties
	public:
		Texture2D(const Texture2DProperties& properties = Texture2DProperties()) : m_textureProperties(properties) {} //!< Constructor
			/*!< \param properties a const Texture2DProperties& - The texture properties */
		virtual ~Texture2D() = default; //!< Destructor

		virtual inline const uint32_t getID() const { return m_textureID; } //!< Get the texture ID
			/*!< \return a const uint32_t - The ID of the texture */

		virtual inline Texture2DProperties& getProperties() { return m_textureProperties; } //!< Get the texture properties
			/*!< \return a Texture2DProperties& - The properties of the texture */

		virtual inline const uint32_t getChannels() const { return m_channels; } //!< Get the texture's number of channels
			/*!< \return a const uint32_t - The number of channels in the texture data */

		virtual void edit(const uint32_t offsetX, const uint32_t offsetY, const uint32_t width, const uint32_t height, const unsigned char* data) = 0; //!< Edit the texture data
			/*!< \param offsetX a const uint32_t - The x offset in memory to edit
			     \param offsetY a const uint32_t - The y offset in memory to edit
				 \param width a const uint32_t - The width of the texture
				 \param height a const uint32_t - The height of the texture
				 \param data a const unsigned char* - A pointer to the data to store */

		virtual void bind(const uint32_t slot = 0) = 0; //!< Bind the texture to a texture unit
			/*!< \param slot a const uint32_t - Bind the texture to texture unit */

		static Texture2D* create(const Texture2DProperties& properties, const uint32_t channels, unsigned char* data); //!< Create a texture providing the pixel data
		static Texture2D* create(const char* filePath, Texture2DProperties properties = Texture2DProperties()); //!< Create a texture from file
	};

	/*! \class CubeMapTexture
	* \brief An API agnostic Cubemap texture
	*/
	class CubeMapTexture
	{
	protected:
		uint32_t m_textureID; //!< Texture ID
	public:
		CubeMapTexture() {} //!< Constructor
		virtual ~CubeMapTexture() = default; //!< Destructor

		virtual inline const uint32_t getID() const { return m_textureID; } //!< Get the texture ID
			/*!< \return a const uint32_t - The ID of the texture */

		virtual void bind(const uint32_t slot = 0) = 0; //!< Bind the texture to a texture unit
			/*!< \param slot a const uint32_t - Bind the texture to texture unit */

		static CubeMapTexture* create(const std::string& folderPath, const std::string& fileType); //!< Create a texture from file
	};
}
#endif