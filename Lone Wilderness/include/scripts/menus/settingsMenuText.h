/*! \file settingsMenuText.h
*
* \brief A script for all the interactable text in the settings menu
*
* \author: Daniel Bullin
*
*/
#ifndef SETTINGSMENUTEXT_H
#define SETTINGSMENUTEXT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"

using namespace Engine;

/*! \class SettingsMenuText
* \brief A script for all the interactable text in the settings menu
*/
class SettingsMenuText : public NativeScript
{
private:
	Text* m_text; //!< A pointer to the text
public:
	SettingsMenuText(); //!< Constructor
	~SettingsMenuText(); //!< Destructor
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif