/*! \file settingsLayer.h
*
* \brief The settings layer containing all entities to do with the settings menu
*
* \author: Daniel Bullin
*
*/
#ifndef SETTINGSLAYER_H
#define SETTINGSLAYER_H

#include "independent/layers/layer.h"

using namespace Engine;

/*! \class SettingsLayer
* \brief A layer for the settings menu entities
*/
class SettingsLayer : public Layer
{
public:
	SettingsLayer(); //!< Constructor
	~SettingsLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
};
#endif