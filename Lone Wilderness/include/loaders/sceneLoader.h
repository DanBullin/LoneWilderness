/*! \file sceneLoader.h
*
* \brief A scene loader class which loads the scene from a JSON file.
*
* \author Daniel Bullin
*
*/
#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "independent/systems/components/scene.h"

using namespace Engine;

/*! \class SceneLoader
* \brief A loader class which loads the scene object by creating all the objects from files
*/
class SceneLoader
{
private:
	static Entity* createNewEntity(const std::string& entitySubType);
public:
	static void load(Scene* scene, const std::string& sceneFolderPath); //!< Load the scene
};

#endif