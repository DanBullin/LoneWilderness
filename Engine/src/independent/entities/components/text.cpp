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
#include "independent/rendering/renderers/renderer2D.h"
#include "systems/systems/fontManager.h"

namespace Engine
{
	//! Text()
	/*!
	\param text a const std::string& - The text
	\param tint a const glm::vec4& - The text colour
	\param fontName a const std::string& - The font name
	*/
	Text::Text(const std::string& text, const glm::vec4& tint, const std::string& fontName) : EntityComponent(ComponentType::Text)
	{
		m_text = "";
		m_font = FontManager::getFontList().begin()->second->getName();
		m_tint = { 1.f, 1.f, 1.f, 1.f };
		setText(text);
		setFont(fontName);
		setColour(tint);
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
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[Text::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Text Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Text: {0}.", m_text);
		ENGINE_TRACE("Tint: R:{0}, G:{1}, B:{2}, A:{3}.", m_tint.r, m_tint.g, m_tint.b, m_tint.a);
		ENGINE_TRACE("Font Name: {0}", m_font);
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
		Font* font = FontManager::getFont(fontName);

		if (font)
			m_font = fontName;
		else
			ENGINE_ERROR("[Text::setFont] An invalid font name was provided. Cannot set new font name.");
	}

	//! getFont()
	/*!
	\return a const std::string& - The font name
	*/
	const std::string& Text::getFont() const
	{
		return m_font;
	}

	//! onRender()
	void Text::onRender()
	{
		if (getParent()->containsComponent<Transform>())
		{
			if (getParent()->containsComponent<NativeScript>())
			{
				getParent()->getComponent<NativeScript>()->onSubmit(Renderers::Renderer2D, "Default");
			}

			Renderer2D::submitText(this, getParent()->getComponent<Transform>()->getModelMatrix());
		}
		else
			ENGINE_ERROR("[Text::onRender] The entity this mesh render is attached to does not have a valid transform.");
	}
}