/*! \file common.h
*
* \brief A common include file containing common includes
*
* \author Daniel Bullin
*
*/
#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <inttypes.h>
#include <array>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <direct.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename S, typename T>
using Map = std::map<S, std::shared_ptr<T>>; //!< Type alias for maps

template<typename T>
using Shared = std::shared_ptr<T>; //!< Shared pointer type alias

#define ENGINE_ERROR(...) Engine::Log::error(__VA_ARGS__) //!< Print error message
#define ENGINE_INFO(...)  Engine::Log::info(__VA_ARGS__) //!< Print info message
#define ENGINE_WARN(...)  Engine::Log::warn(__VA_ARGS__) //!< Print warn message
#define ENGINE_TRACE(...)  Engine::Log::trace(__VA_ARGS__) //!< Print trace message
#define ENGINE_DEBUG(...)  Engine::Log::debug(__VA_ARGS__) //!< Print debug message
#define ENGINE_RELEASE(...)  Engine::Log::release(__VA_ARGS__) //!< Print release mode message
#define ENGINE_FILE(...)  Engine::Log::file(__VA_ARGS__) //!< File message

#define TIME_FUNCTION(x, y) Engine::TimerSystem::startTimer(x); y; Engine::TimerSystem::stopTimer(x, false, true); //!< Macro to time a function
#define GET_TIME(x, y, z)  Engine::TimerSystem::getTime(x, y, z) //!< Macro to get a time from the timer system

#define PAUSE_ONKEYPRESS(x) Engine::InputPoller::isKeyPressed(x)

/*! \enum Renderers
* \brief The different types of renderers
*/
enum class Renderers
{
	Renderer3D = 0,
	Renderer2D = 1
};

#endif