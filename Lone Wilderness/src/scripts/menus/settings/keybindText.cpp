/*! \file keybindText.h
*
* \brief A script for the text allow keybinds to be changed
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/settings/keybindText.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/timerSystem.h"

KeybindText* KeybindText::s_keybindText = nullptr;

//! KeybindText()
KeybindText::KeybindText()
{
	m_function = PlayerConfig::toType("None");
	m_text = nullptr;
	m_changing = false;
	TimerSystem::startTimer("KeybindSwitch");
}

//! KeybindText()
/*!
\param scriptData a nlohmann::json - The script data in a json object
*/
KeybindText::KeybindText(nlohmann::json scriptData)
{
	m_function = PlayerConfig::toType(scriptData["playerConfig"].get<std::string>());
	m_text = nullptr;
	m_changing = false;
	TimerSystem::startTimer("KeybindSwitch");
}

//! ~KeybindText()
KeybindText::~KeybindText()
{
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void KeybindText::onPostUpdate(const float timestep, const float totalTime)
{
	if (!m_text)
	{
		m_text = getParent()->getComponent<Text>();
		if (m_text)
		{
			std::string str = m_text->getText();
			str.at(str.length() - 1) = (char)Settings::getKeys(m_function).first;
			m_text->setText(str);
		}
	}

	if (s_keybindText == this)
	{
		if (m_text)
		{
			std::string str = m_text->getText();
			char lastChar = str.at(str.length() - 1);

			if (TimerSystem::getTime("KeybindSwitch", false, true) > 0.5f)
			{
				if (lastChar == '_')
					lastChar = ' ';
				else
					lastChar = '_';

				str.at(str.length() - 1) = lastChar;

				m_text->setText(str);
				TimerSystem::startTimer("KeybindSwitch");
			}
		}
	}
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void KeybindText::onMousePress(MousePressedEvent & e, const float timestep, const float totalTime)
{
	bool containsMouse = getParent()->containsPoint(InputPoller::getMousePosition());

	if (m_text)
	{
		if (containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			m_text->setColour({ 1.f, 0.7f, 0.f, 1.f });
	}
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void KeybindText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	std::string entityName = getParent()->getName();
	bool containsMouse = getParent()->containsPoint(mousePosition);
	std::string sceneName = getParent()->getParentScene()->getName();

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse && !s_keybindText)
	{
		if (!m_changing)
		{
			m_changing = true;
			s_keybindText = this;
		}
		else
		{
			m_changing = false;
			s_keybindText = nullptr;
		}
	}

	if (m_text)
	{
		if (!containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			m_text->setColour({ 1.f, 1.f, 1.f, 1.f });
	}
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void KeybindText::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
	bool containsMouse = getParent()->containsPoint(e.getPos());

	if (m_text)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 0.5f, 0.85f, 1.f });
		else if (!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 1.0f, 1.f, 1.f });
	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void KeybindText::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (s_keybindText == this)
	{
		if (m_text)
		{
			std::string str = m_text->getText();
			str.at(str.length() - 1) = e.getKeyCode();
			m_text->setText(str);
			Settings::bindKey(m_function, e.getKeyCode(), 0);
		}
		s_keybindText = nullptr;
	}
}