/*! \file system.h
*
* \brief The base functionality of a system
*
* \author Daniel Bullin
*
*/
#ifndef SYSTEM_H
#define SYSTEM_H

#include "independent/core/common.h"

namespace Engine 
{
	namespace Systems
	{
		/*! \enum Type
		* \brief The different types of systems
		*/
		enum Type
		{
			Logger = 1, //!< A log to console or files
			Randomiser = 2, //!< A random number generator
			TimerSystem = 3, //!< Create and use timers
			WindowAPISystem = 4, //!< The windowing API
			WindowManager = 5, //!< A manager for windows
			EventManager = 6, //!< An event manager
			ResourceManager = 7, //!< A resource manager
			SceneManager = 8, //!< A scene manager
			RenderSystem = 9, //!< A render system
			FontManager = 10 //!< A Font manager
		};
	}

	/*! \class System
	* \brief Base class for all systems
	*/
	class System
	{
	private:
		Systems::Type m_systemType; //!< The system type
	public:
		System(const Systems::Type type); //!< Constructor
		virtual ~System(); //!< Destructor
		const Systems::Type getSystemType() const; //!< Get the type of the system

		virtual void start() = 0; //!< Start the system
		virtual void stop() = 0; //!< Stop the system
	};
}
#endif