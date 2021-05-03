/*! \file resourcesScript.h
*
* \brief A resources script
*
* \author: Daniel Bullin
*
*/
#ifndef RESOURCESSCRIPT_H
#define RESOURCESSCRIPT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"
#include "independent/systems/components/thread.h"

using namespace Engine;

/*! \class ResourcesScript
* \brief A script to load resources
*/
class ResourcesScript : public NativeScript
{
private:
	bool m_uploadedModels; //!< A bool to determine whether we have uploaded the models
	Text* m_loadingText; //!< The loading text to update
	Thread* m_thread; //!< The resource loading thread
public:
	ResourcesScript(); //!< Constructor
	~ResourcesScript(); //!< Destructor
	void onAttach() override; //!< Called when attached to the entity
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
};
#endif