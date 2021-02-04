/*! \file defaultPass.h
*
* \brief A default render pass
*
* \author Daniel Bullin
*
*/
#ifndef DEFAULTPASS_H
#define DEFAULTPASS_H

#include "independent/rendering/renderPasses/renderPass.h"
#include "independent/rendering/frameBuffer.h"

namespace Engine
{
	/*! \class DefaultPass
	* \brief A default render pass
	*/
	class DefaultPass : public RenderPass
	{
	public:
		DefaultPass(); //!< Constructor
		virtual ~DefaultPass(); //!< Destructor

		virtual void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
	};
}
#endif