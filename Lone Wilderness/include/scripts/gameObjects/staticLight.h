/*! \file staticLight.h
*
* \brief A script for a static light in the scene
*
* \author: Daniel Bullin
*
*/
#ifndef STATICLIGHT_H
#define STATICLIGHT_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class StaticLight
* \brief A script for a static light in the scene
*/
class StaticLight : public NativeScript
{
public:
	StaticLight(); //!< Constructor
	~StaticLight(); //!< Destructor
	void onSubmit(const Renderers renderer, const std::string& renderState) override; //!< Call upon render if mesh render component attached
};
#endif