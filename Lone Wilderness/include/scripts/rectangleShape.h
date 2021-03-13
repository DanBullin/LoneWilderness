/*! \file rectangleShape.h
*
* \brief A RectangleShape dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class RectangleShape
* \brief A RectangleShape script
*/
class RectangleShape : public NativeScript
{
public:
	RectangleShape(); //!< Constructor
	~RectangleShape(); //!< Destructor
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif