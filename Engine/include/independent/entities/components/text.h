/*! \file text.h
*
* \brief A text component to render text
*
* \author Daniel Bullin
*
*/
#ifndef TEXT_H
#define TEXT_H

#include "independent/entities/entityComponent.h"

namespace Engine
{
	/*! \class Text
	* \brief A text component to render text
	*/
	class Text : public EntityComponent
	{
	private:
		std::string m_text; //!< The text to render
		std::string m_font; //!< The name of the font to use
		glm::vec4 m_tint; //!< The colour of the text
		glm::vec3 m_localPosition; //!< The local position of the text to the entity
		float m_localOrientation; //!< The local rotation of the text to the entity
		glm::vec2 m_localScale; //!< The local scale of the text to the entity
	public:
		Text(const char* text, const glm::vec4& tint, const char* fontName, const glm::vec3& localPos, const float localOrientation, const glm::vec2& localScale); //!< Constructor
		~Text(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details

		void setText(const std::string& text); //!< Set the text
		const std::string& getText() const; //!< Get the text

		void setColour(const glm::vec4& colour); //!< Set the text colour
		const glm::vec4& getColour() const; //!< Get the text colour

		void setFont(const std::string& fontName); //!< Set the font name
		const std::string& getFont() const; //!< Get the font name

		bool containsPoint(const glm::vec2& coordinate); //!< Returns whether the point is inside this text's bounding box

		void setLocalPosition(const glm::vec3& newPos); //!< Set the local position
		glm::vec3 getLocalPosition(); //!< Get the local position
		void setLocalOrientation(const float newOrientation); //!< Set the local orientation
		float getLocalOrientation(); //!< Get the local orientation
		void setLocalScale(const glm::vec2& newScale); //!< Set the local scale
		glm::vec2 getLocalScale(); //!< Get the local scale

		glm::mat4 getModelMatrix(); //!< Get the model matrix of the text geometry
	};
}
#endif