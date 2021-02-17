/*! \file text.cpp
*
* \brief A text component to render text
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/text.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! Text()
	/*!
	\param text a const char* - The text
	\param tint a const glm::vec4& - The text colour
	\param fontName a const char* - The font name
	\param localPos a const glm::vec3& - The local position
	\param localOrientation a const float - The local orientation
	\param localScale a const glm::vec2& - The local scale
	*/
	Text::Text(const char* text, const glm::vec4& tint, const char* fontName, const glm::vec3& localPos, const float localOrientation, const glm::vec2& localScale) : EntityComponent(ComponentType::Text),
		m_text(text), m_font(fontName), m_tint(tint), m_localPosition(localPos), m_localOrientation(localOrientation), m_localScale(localScale)
	{
	}

	//! ~Text()
	Text::~Text()
	{

	}

	//! onAttach()
	void Text::onAttach()
	{
	}

	//! onDetach
	void Text::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void Text::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void Text::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Text Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Text: {0}.", m_text);
		ENGINE_TRACE("Tint: R:{0}, G:{1}, B:{2}, A:{3}.", m_tint.r, m_tint.g, m_tint.b, m_tint.a);
		ENGINE_TRACE("Font Name: {0}", m_font);
		ENGINE_TRACE("Local Position: {0}, {1}.", m_localPosition.x, m_localPosition.y);
		ENGINE_TRACE("Local Orientation: {0}.", m_localOrientation);
		ENGINE_TRACE("Local Scale: {0}, {1}.", m_localScale.x, m_localScale.y);
		ENGINE_TRACE("==========================================");
	}

	//! setText()
	/*!
	\param text a const std::string& - The text
	*/
	void Text::setText(const std::string& text)
	{
		m_text = text;
	}

	//! getText()
	/*!
	\return a const std::string& - The text
	*/
	const std::string& Text::getText() const
	{
		return m_text;
	}

	//! setColour()
	/*!
	\param colour a const glm::vec4& - The text colour
	*/
	void Text::setColour(const glm::vec4& colour)
	{
		m_tint = colour;
	}

	//! getColour()
	/*!
	\return a const glm::vec4& - The text colour
	*/
	const glm::vec4& Text::getColour() const
	{
		return m_tint;
	}

	//! setText()
	/*!
	\param fontName a const std::string& - The font name
	*/
	void Text::setFont(const std::string& fontName)
	{
		m_font = fontName;
	}

	//! getFont()
	/*!
	\return a const std::string& - The font name
	*/
	const std::string& Text::getFont() const
	{
		return m_font;
	}

	//! containsPoint()
	/*!
	\param coordinate a const glm::vec2& - The coordinate to check
	\return a bool - Is this coordinate within this bounding box
	*/
	bool Text::containsPoint(const glm::vec2& coordinate)
	{
		// Need to convert bottom left origin to calc top left and right, manually input scale and tune values as text size isnt known
		glm::vec2 pos = getParent()->getComponent<Transform2D>()->getPosition() + glm::vec2(m_localPosition);
		glm::vec2 topLeft = pos - glm::vec2(0.f, m_localScale.y);
		glm::vec2 bottomRight = pos + glm::vec2(m_localScale.x, 0.f);

		if (coordinate.x >= topLeft.x && coordinate.x <= bottomRight.x)
			if (coordinate.y >= topLeft.y && coordinate.y <= bottomRight.y)
				return true;

		return false;
	}

	//! setLocalPosition()
	/*!
	\param newPos a const glm::vec3& - The new position
	*/
	void Text::setLocalPosition(const glm::vec3& newPos)
	{
		m_localPosition = newPos;
	}

	//!	getLocalPosition()
	/*!
	\return a glm::vec3 - The local position of the text
	*/
	glm::vec3 Text::getLocalPosition()
	{
		return m_localPosition;
	}

	//! setLocalOrientation()
	/*!
	\param newOrientation a const float - The new orientation
	*/
	void Text::setLocalOrientation(const float newOrientation)
	{
		m_localOrientation = newOrientation;
	}

	//!	getLocalOrientation()
	/*!
	\return a float - The local orientation of the text
	*/
	float Text::getLocalOrientation()
	{
		return m_localOrientation;
	}

	//! setLocalScale()
	/*!
	\param newScale a const glm::vec2& - The new scale
	*/
	void Text::setLocalScale(const glm::vec2& newScale)
	{
		m_localScale = newScale;
	}

	//!	getLocalScale()
	/*!
	\return a glm::vec2 - The local scale of the text
	*/
	glm::vec2 Text::getLocalScale()
	{
		return m_localScale;
	}

	//! getModelMatrix()
	/*!
	\return a glm::mat4 - The model transformation matrix
	*/
	glm::mat4 Text::getModelMatrix()
	{
		Transform2D* trans = getParent()->getComponent<Transform2D>();
		// Order: Translate then Rotation then Scale
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(trans->getPosition(), 0.f));
		model = glm::translate(model, m_localPosition);
		model = glm::rotate(model, glm::radians(trans->getOrientation()), glm::vec3(0.f, 0.f, 1.f));
		model = glm::rotate(model, glm::radians(m_localOrientation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, glm::vec3(trans->getScale(), 0.f));
		model = glm::scale(model, glm::vec3(m_localScale, 0.f));
		return model;
	}
}