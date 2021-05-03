/*! \file water.h
*
* \brief A script to create a water shader
*
* \author: Daniel Bullin
*
*/
#ifndef WATER_H
#define WATER_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/transform.h"
#include "terrain/chunkManager.h"

using namespace Engine;

/*! \class Water
* \brief A water script
*/
class Water : public NativeScript
{
private:
	static ChunkManager* s_chunkManager; //!< A chunk manager
public:
	Water(); //!< Constructor
	~Water(); //!< Destructor

	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onRender(const Renderers renderer, const std::string& renderState); //!< On Render
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif