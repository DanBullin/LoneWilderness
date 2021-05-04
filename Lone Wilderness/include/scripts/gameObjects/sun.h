/*! \file sun.h
*
* \brief A script for the rising and setting of the sun
*
* \author: Daniel Bullin
*
*/
#ifndef SUN_H
#define SUN_H

#include "independent/entities/components/nativeScript.h"
#include "scripts/gameTimer.h"

using namespace Engine;

/*! \class Sun
* \brief A script for the sun
*/
class Sun : public NativeScript
{
private:
	GameTimer* m_timer;
	float m_cycleTime;
	Material* m_skyboxMaterial;
public:
	Sun(); //!< Constructor
	~Sun(); //!< Destructor
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onSubmit(const Renderers renderer, const std::string& renderState) override; //!< Call upon render if mesh render component attached
};
#endif