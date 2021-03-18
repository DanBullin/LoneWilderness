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
	/*! \enum SystemType
	* \brief The different types of systems
	*/
	enum class SystemType
	{
		Logger, Randomiser, TimerSystem, WindowAPISystem, WindowManager, EventManager, ResourceManager, SceneManager, FontManager, RenderSystem, ThreadManager
	};

	/*! \class System
	* \brief Base class for all systems
	*/
	class System
	{
	private:
		SystemType m_systemType; //!< The system type
	public:
		System(const SystemType type); //!< Constructor
		virtual ~System(); //!< Destructor
		const SystemType getSystemType() const; //!< Get the type of the system

		virtual void start() = 0; //!< Start the system
		virtual void stop() = 0; //!< Stop the system
	};
}
#endif