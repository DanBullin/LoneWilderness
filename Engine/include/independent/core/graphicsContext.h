/*! \file graphicsContext.h 
*
* \brief A graphics context interface
*
* \author Daniel Bullin
*
*/
#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

namespace Engine
{
	/*! \class GraphicsContext
	* \brief A graphics context interface
	*/
	class GraphicsContext
	{
	public:
		virtual void init() = 0; //!< Init the graphics context for the given windowing api
		virtual void swapBuffers() = 0; //!< Swap the front and back buffers
		virtual void makeCurrent() = 0; //!< Make this graphics context the current context
		virtual void updateViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) = 0; //!< Update the viewport
			/*! \param x a const uint32_t - The lower left corner of the rectangle viewport
				\param y a const uint32_t - The lower left corner of the rectangle viewport
				\param width a const uint32_t - The width of the rectangle viewport
				\param height a const uint32_t - The height of the rectangle viewport */
	};
}
#endif