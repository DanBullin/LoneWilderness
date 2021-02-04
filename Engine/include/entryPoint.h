/*! \file entryPoint.h
*
* \brief Entry point for the engine application
*
* \author DMU Course material
*
*/
#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "independent/core/application.h"

extern Engine::Application* Engine::startApplication(); //!< Global variable available everywhere with external linkage

//! main()
/*!
\param argc an int - Number of strings pointed to by argv
\param argv a char** - Array of c-string pointers
\return an int - The exit code
*/
int main(int argc, char** argv)
{
	// Start the application
	auto application = Engine::startApplication();
	application->run();

	// Application has stopped running, delete the pointer
	delete application;
	return 0;
}
#endif