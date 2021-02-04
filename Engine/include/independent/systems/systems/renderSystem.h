/*! \file renderSystem.h
*
* \brief A render system which manages rendering (both 2D & 3D)
*
* \author Daniel Bullin
*
*/
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "independent/core/common.h"
#include "independent/systems/system.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	/*! \class RenderSystem
	* \brief A render system which manages rendering (both 2D & 3D)
	*/
	class RenderSystem : public System
	{
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::shared_ptr<TextureUnitManager> s_unitManager; //!< The texture unit manager
		static std::array<int32_t, 16> s_unit; //!< The texture units
	public:
		RenderSystem(); //!< Constructor
		~RenderSystem(); //!< Destructor

		virtual void start() override; //!< Start the system
		virtual void stop() override; //!< Stop the system

		static void initialise(); //!< Initialise the render system after all systems have been added and started

		static void onUpdate(const float timestep, const float totalTime); //!< Update
		static void onRender(Scene* scene); //!< Render the scene
	};
}
#endif