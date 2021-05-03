/*! \file pauseMenuText.h
*
* \brief A script for all the interactable text in the pause menu
*
* \author: Daniel Bullin
*
*/
#ifndef PAUSEMENUTEXT_H
#define PAUSEMENUTEXT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"

using namespace Engine;

/*! \class PauseMenuText
* \brief A script for all the interactable text in the pause menu
*/
class PauseMenuText : public NativeScript
{
private:
	Text* m_text; //!< A pointer to the text
	void loadScene(const std::string& sceneName); //!< Load a scene
public:
	PauseMenuText(); //!< Constructor
	~PauseMenuText(); //!< Destructor
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif