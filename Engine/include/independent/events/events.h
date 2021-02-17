/*! \file events.h 
*
* \brief A header file including all types of events
*
* \author DMU Course material
*
*/
#ifndef EVENTS_H
#define EVENTS_H

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/events/GLFWCodes.h"
#endif

#include "independent/events/windowEvents.h"
#include "independent/events/keyEvents.h"
#include "independent/events/mouseEvents.h"
#include "independent/events/inputPoller.h"

#endif