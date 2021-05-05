/*! \file placeObject.h
*
* \brief A script which places objects in the world
*
* \author: Daniel Bullin
*
*/
#ifndef PLACEOBJECT_H
#define PLACEOBJECT_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

class Player;

/*! \class PlaceObject
* \brief A script which places objects in the world
*/
class PlaceObject : public NativeScript
{
private:
	Entity* m_camera;
	Entity* m_currentEntity;
	Player* m_player;
	uint32_t number = 0;
	bool m_final = false;
public:
	PlaceObject(); //!< Constructor
	~PlaceObject(); //!< Destructor
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override;
};
#endif