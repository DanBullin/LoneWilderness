/*! \file subTexture.h
*
* \brief A subtexture of a base texture
*
* \author DMU Course material
*
*/
#ifndef SUBTEXTURE_H
#define SUBTEXTURE_H

#include "independent/rendering/textures/texture.h"

namespace Engine
{
	//! convertCoordsFromBottomLeftSystem()
	/*!
	\param UVStart a glm::vec2& - The UV start coordinates
	\param UVEnd a glm::vec2& - The UV end coordinates
	*/
	static void convertCoordsToBottomLeftSystem(glm::vec2& UVStart, glm::vec2& UVEnd)
	{
		UVStart = glm::vec2(0.f, 1.f);
		UVEnd = glm::vec2(1.f, 0.f);
	}

	/*! \class SubTexture
	* \brief A subtexture holding texture and UV data
	*/
	class SubTexture : public Resource
	{
	private:
		Texture2D* m_baseTexture; //!< The base texture
		glm::vec2 m_UVStart; //!< UV Mapping start coordinate
		glm::vec2 m_UVEnd; //!< UV Mapping end coordinate
		glm::ivec2 m_size; //!< Size in pixels of the subtexture
	public:
		SubTexture(const std::string& subTextureName, Texture2D* baseTexture, const glm::vec2& UVStart, const glm::vec2& UVEnd, const bool convertToBottomLeft); //!< Constructor
		~SubTexture(); //!< Destructor

		inline glm::vec2 getUVStart() const { return m_UVStart; } //!< Get the UV start coordinates
			/*!< \return a glm::vec2 - The UV start coorindates */
		inline glm::vec2 getUVEnd() const { return m_UVEnd; } //!< Get the UV end coordinates
			/*!< \return a glm::vec2 - The UV end coorindates */
		inline glm::ivec2 getSize() const { return m_size; } //!< Get the size in pixels of the subtexture
			/*!< \return a glm::ivec2 - The size of the subtexture in pixels */
		inline glm::vec2 getSizef() const { return { static_cast<float>(m_size.x), static_cast<float>(m_size.y) }; } //!< Get the size in pixels of subtexture as float
			/*!< \return a glm::vec2 - The size of the subtexture in pixels */
		inline const uint32_t getWidth() const { return m_size.x; } //!< Get the texture width
			/*!< \return a const uint32_t - The width of the texture */
		inline const uint32_t getHeight() const { return m_size.y; } //!< Get the texture height
			/*!< \return a const uint32_t - The height of the texture */
		inline const float getWidthf() const { return static_cast<float>(m_size.x); } //!< Get the texture width
			/*!< \return a const float - The width of the texture */
		inline const float getHeightf() const { return static_cast<float>(m_size.y); } //!< Get the texture height
			/*!< \return a const float - The height of the texture */

		inline Texture2D* getBaseTexture() { return m_baseTexture; } //!< Get the base texture
			/*!< \return a Texture2D* - The base texture */

		void setBaseTexture(Texture2D* texture, const glm::vec2& UVStart, const glm::vec2& UVEnd, const bool convertToBottomLeft); //!< Set the base texture
		void setSize(const glm::ivec2& size); //!< Set the subtexture size
		void setUVs(const glm::vec2& UVStart, const glm::vec2& UVEnd); //!< Set the subtexture UVs

		const glm::vec2 transformUV(const glm::vec2& uv); //!< Transform original UVs to atlased UV
		const std::array<int16_t, 2> transformUV(const std::array<int16_t, 2>& uv); //!< Transform original UVs to atlased UV

		const int16_t transformU(const int16_t u); //!< Transform original U to atlased U
		const int16_t transformV(const int16_t v); //!< Transform original V to atlased V

		void printDetails() override; //!< Print the resource details
	};
}
#endif