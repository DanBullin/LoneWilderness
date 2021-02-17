/*! \file rectangleShape.h
*
* \brief A RectangleShape dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H

#include "independent/entities/entity.h"

using namespace Engine;

/*! \class RectangleShape
* \brief A RectangleShape dummy class
*/
class RectangleShape : public Entity
{
public:
	RectangleShape(); //!< Constructor
	~RectangleShape(); //!< Destructor
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif