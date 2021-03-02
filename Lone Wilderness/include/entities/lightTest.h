/*! \file lightTest.h
*
* \brief A light example dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef LIGHTTEST_H
#define LIGHTTEST_H

#include "independent/entities/entity.h"

using namespace Engine;

/*! \class LightTest
* \brief A light test dummy class
*/
class LightTest : public Entity
{
private:
	float m_distance = 0.f;
	float m_increment = 0.03f;
	glm::vec3 m_originalPos;
	bool m_first = true;
	bool m_light = true;
public:
	LightTest(); //!< Constructor
	~LightTest(); //!< Destructor
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onRender(const Renderers renderer); //!< Call upon render if mesh render component attached
};
#endif