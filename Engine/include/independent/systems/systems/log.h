/*! \file log.h
*
* \brief A logger system which prints information to various outputs
*
* \author DMU Course material
*
*/
#ifndef LOG_H
#define LOG_H

#include <spdlog/spdlog.h>
#include "independent/core/common.h"
#include "independent/systems/system.h"

namespace Engine
{
	/*! \class Log
	* \brief A log class using spdlog which prints information to various outputs (Console, file, etc)
	*/
	class Log : public System {
	private:
		static bool s_enabled; //!< Is this logger system enabled
		static Shared<spdlog::logger> s_consoleLogger; //!< Console logger
		static Shared<spdlog::logger> s_fileLogger; //!< File logger
	public:
		Log(); //!< Constructor
		~Log(); //!< Destructor
		void start() override; //!< Start the logger
		void stop() override; //!< Stop the logger

		void enableFileLogging(const bool enable); //!< Enable/Disable the file logging

		template<class ...Args> static void info(Args&&... args); //!< Print info to the console
		template<class ...Args> static void trace(Args&&... args); //!< Print a trace message to the console
		template<class ...Args> static void warn(Args&&... args); //!< Print a warn message to the console
		template<class ...Args> static void debug(Args&&... args); //!< Print a debug message to the console
		template<class ...Args> static void error(Args&&... args); //!< Print an error message to the console
		template<class ...Args> static void release(Args&&... args); //!< Print a trace message in release mode to the console
		template<class ...Args> static void file(Args&&... args); //!< Print information to the log files
	};

	template<class ...Args>
	//! debug()
	/*!
	\param args a Args&& - Parameter pack
	*/
	static void Log::debug(Args&&... args)
	{
#ifdef NG_DEBUG
		if (s_enabled && s_consoleLogger)
			s_consoleLogger->debug(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	//! error()
	/*!
	\param args a Args&& - Parameter pack
	*/
	static void Log::error(Args&&... args)
	{
#ifdef NG_DEBUG
		if (s_enabled && s_consoleLogger)
			s_consoleLogger->error(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	//! info()
	/*!
	\param args a Args&& - Parameter pack
	*/
	static void Log::info(Args&&... args)
	{
#ifdef NG_DEBUG
		if (s_enabled && s_consoleLogger)
			s_consoleLogger->info(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	//! trace()
	/*!
	\param args a Args&& - Parameter pack
	*/
	static void Log::trace(Args&&... args)
	{
#ifdef NG_DEBUG
		if (s_enabled && s_consoleLogger)
			s_consoleLogger->trace(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	//! warn()
	/*!
	\param args a Args&& - Parameter pack
	*/
	static void Log::warn(Args&&... args)
	{
#ifdef NG_DEBUG
		if (s_enabled && s_consoleLogger)
			s_consoleLogger->warn(std::forward<Args>(args) ...);
#endif
	}

	template<class ...Args>
	//! release()
	/*!
	\param args a Args&& - Parameter pack
	*/
	static void Log::release(Args&&... args)
	{
		if (s_enabled && s_consoleLogger)
			s_consoleLogger->trace(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	//! file()
	/*!
	\param args a Args&& - Parameter pack
	*/
	static void Log::file(Args&&... args)
	{
		// Check if file logger has a valid pointer
		if (s_enabled && s_fileLogger)
			s_fileLogger->trace(std::forward<Args>(args) ...);
		else
			ENGINE_ERROR("[Log::file] File logging is disabled.");
	}
}
#endif