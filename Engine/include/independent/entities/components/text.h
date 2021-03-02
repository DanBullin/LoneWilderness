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
	public:
		Text(const std::string& text, const glm::vec4& tint, const std::string& fontName); //!< Constructor
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
	};
}
#endif