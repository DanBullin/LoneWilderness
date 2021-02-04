/*! \file renderAPI.cpp
*
* \brief The rendering API
*
* \author DMU Course material
*
*/

#include "independent/rendering/renderAPI.h"

namespace Engine
{
	GraphicsAPI RenderAPI::s_API = Engine::GraphicsAPI::OpenGL; //!< Initialise the graphics API
}