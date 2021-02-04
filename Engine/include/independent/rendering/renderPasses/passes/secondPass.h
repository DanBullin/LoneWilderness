/*! \file secondPass.h
*
* \brief A Second render pass
*
* \author Daniel Bullin
*
*/
#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class SecondPass
	* \brief A second render pass
	*/
	class SecondPass : public RenderPass
	{
	public:
		SecondPass(); //!< Constructor
		virtual ~SecondPass(); //!< Destructor

		virtual void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
	};
}
#endif