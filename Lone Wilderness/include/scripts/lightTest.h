/*! \file lightTest.h
*
* \brief A light example dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef LIGHTTEST_H
#define LIGHTTEST_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class LightTest
* \brief A light test dummy class
*/
class LightTest : public NativeScript
{
private:
	float m_distance = 0.f;
	float m_increment = 0.03f;
	glm::vec3 m_originalPos;
	bool m_first = true;
public:
	LightTest(); //!< Constructor
	~LightTest(); //!< Destructor
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onSubmit(const Renderers renderer) override; //!< Call upon render if mesh render component attached
};
#endif