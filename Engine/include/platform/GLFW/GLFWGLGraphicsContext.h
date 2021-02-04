/*! \file GLFWGLGraphicsContext.h 
*
* \brief A GLFW OpenGL graphics context
*
* \author DMU Course material
*
*/
#ifndef GLFWGLGRAPHICSCONTEXT_H
#define GLFWGLGRAPHICSCONTEXT_H

#include "independent/core/graphicsContext.h"

namespace Engine {
	/*! \class GLFWGLGraphicsContext
	* \brief An GLFW OpenGL graphics context
	*/
	class GLFWGLGraphicsContext : public GraphicsContext
	{
	private:
		GLFWwindow* m_window; //!< Pointer to GLFW window
	public:
		GLFWGLGraphicsContext(GLFWwindow* nativeWindow) : m_window(nativeWindow) {} //!< Constructor
			/*!< \param nativeWindow a GLFWwindow* - A pointer to a GLFW window */
		virtual void init() override; //!< Init the graphics context for the given windowing api
		virtual void swapBuffers() override; //!< Swap the front and back buffers
		virtual void makeCurrent() override; //!< Make this graphics context the current context
		virtual void updateViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) override; //!< Update the viewport
	};
}
#endif