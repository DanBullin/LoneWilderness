/*! \file settingsScript.h
*
* \brief A script to show the settings menu
*
* \author: Daniel Bullin
*
*/
#ifndef SETTINGSSCRIPT_H
#define SETTINGSSCRIPT_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class SettingsScript
* \brief A script which shows the settings menu
*/
class SettingsScript : public NativeScript
{
public:
	SettingsScript(); //!< Constructor
	~SettingsScript(); //!< Destructor
	void onAttach() override; //!< On attach
};
#endif