/*! \file cyborg.h
*
* \brief A Cyborg dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef CYBORG_H
#define CYBORG_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/characterController.h"

using namespace Engine;

/*! \class Cyborg
* \brief A cyborg script
*/
class Cyborg : public NativeScript
{
private:
	CharacterController* m_controller; //!< Pointer to character controller
	glm::vec3 originalScale = glm::vec3(0.f, 0.f, 0.f);
public:
	Cyborg(); //!< Constructor
	~Cyborg(); //!< Destructor
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override;
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif