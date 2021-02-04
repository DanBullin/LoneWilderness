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
private:

public:
	RectangleShape(); //!< Constructor
	~RectangleShape(); //!< Destructor
	virtual void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	virtual void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	virtual void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
};
#endif