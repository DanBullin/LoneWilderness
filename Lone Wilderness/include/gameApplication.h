/*! \file gameApplication.h
*
* \brief The game application
*
* \author Daniel Bullin
*
*/
#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include "entrypoint.h"

using namespace Engine;

/*! \class GameApplication
* \brief A subclass of Application, creating an instance of the engine
*/
class GameApplication : public Application
{
public:
	GameApplication(); //!< Constructor
	~GameApplication() override; //!< Destructor
};
#endif