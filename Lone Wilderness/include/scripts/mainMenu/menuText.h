/*! \file menuText.h
*
* \brief A UI Text entity class for the main menu
*
* \author: Daniel Bullin
*
*/
#ifndef MENUTEXT_H
#define MENUTEXT_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class MenuText
* \brief A MenuText class
*/
class MenuText : public NativeScript
{
public:
	MenuText(); //!< Constructor
	~MenuText(); //!< Destructor
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif