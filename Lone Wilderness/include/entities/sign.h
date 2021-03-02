/*! \file sign.h
*
* \brief A Neon Sign environmental entity
*
* \author: Jamie Hall
*
*/

#ifndef SIGN_H
#define SIGN_H

#include "independent/entities/entity.h"

using namespace Engine;
/*! \class Sign
* \brief A Neon Sign environmental entity
*/
class Sign : public Entity
{
public:
	Sign(); //!< Constructor
	~Sign(); //!< Deconstructor

	virtual void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	virtual void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	virtual void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release

};
#endif