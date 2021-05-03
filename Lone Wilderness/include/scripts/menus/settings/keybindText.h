/*! \file keybindText.h
*
* \brief A script for the text allow keybinds to be changed
*
* \author: Daniel Bullin
*
*/
#ifndef KEYBINDTEXT_H
#define KEYBINDTEXT_H

#include <json.hpp>
#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"
#include "settings/settings.h"

using namespace Engine;

/*! \class KeybindText
* \brief A script for the text allow keybinds to be changed
*/
class KeybindText : public NativeScript
{
private:
	static KeybindText* s_keybindText; //!< The keybind currently being changed
	bool m_changing; //!< Are we currently changing this keybind
	Text* m_text; //!< A pointer to the text
	PlayerConfig::ConfigData m_function; //!< The function this keybind text changes
public:
	KeybindText(); //!< Constructor
	KeybindText(nlohmann::json scriptData); //!< Constructor
	~KeybindText(); //!< Destructor
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif