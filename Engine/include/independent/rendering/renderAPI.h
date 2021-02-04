/*! \file renderAPI.h
*
* \brief The rendering API
*
* \author DMU Course material
*
*/
#ifndef RENDERAPI_H
#define RENDERAPI_H

namespace Engine
{
	/*! \enum GraphicsAPI
	* \brief The graphics GraphicsAPI
	*/
	enum class GraphicsAPI { None = 0, OpenGL = 1, Direct3D = 2, Vulkan = 3 };

	/*! \class RenderAPI
	* \brief The API for our engine at runtime
	*/
	class RenderAPI
	{
	private:
		static GraphicsAPI s_API; //!< The current API
	public:
		inline static GraphicsAPI getAPI() { return s_API; } //!< Get the current API
			/*!< \return a API - The current API */
	};
}
#endif