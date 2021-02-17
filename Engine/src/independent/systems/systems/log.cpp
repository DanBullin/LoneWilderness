/*! \file log.h
*
* \brief A logger system which prints information to various outputs
*
* \author DMU Course material
*
*/
#include "independent/systems/systems/log.h"

namespace Engine 
{
	bool Log::s_enabled = false; //!< Set to false
	std::shared_ptr<spdlog::logger> Log::s_consoleLogger = nullptr; //!< Initialise the console logger
	std::shared_ptr<spdlog::logger> Log::s_fileLogger = nullptr; //!< Initialise the file logger

	//! Log()
	Log::Log() : System(SystemType::Logger)
	{
	}

	//! ~Log()
	Log::~Log()
	{
	}

	//! start()
	void Log::start()
	{
		// Start system if its currently disabled
		if (!s_enabled)
		{
			spdlog::set_pattern("%^[%T]: %v%$");
			spdlog::set_level(spdlog::level::trace);
			s_consoleLogger = spdlog::stderr_color_mt("Console");
			s_enabled = true;
			ENGINE_INFO("[Log::start] Starting the logger system.");
		}
	}

	//! stop()
	void Log::stop()
	{
		// Stop system if its currently enabled
		if (s_enabled)
		{
			// Print stopping message and reset pointer
			ENGINE_INFO("[Log::stop] Stopping the logger system.");

			// Remove loggers from spdlog registry
			spdlog::drop_all();
			s_consoleLogger.reset();
			s_consoleLogger = nullptr;
			s_fileLogger.reset();
			s_fileLogger = nullptr;
			s_enabled = false;
		}
	}

	//! enableFileLogging()
	/*!
	\param enable a const bool - Enable/Disable the file logging
	*/
	void Log::enableFileLogging(const bool enable)
	{
		if (s_enabled)
		{
			// If it needs to be enabled
			if (enable)
			{
				// Set up the file logging
				// Stored in sandbox/logs/
				char filePath[256] = "logs/";
				char time[128];

				try
				{
					const std::time_t t = std::time(nullptr);
					struct tm* timeInfo;
					timeInfo = localtime(&t);
					// Format: [DAY/MONTH/YEAR] HH.MM.SS
					std::strftime(time, sizeof(time), "[%d.%m.%y] %I.%M.%S", timeInfo);
					strcat_s(filePath, time);
					strcat_s(filePath, ".txt");
					s_fileLogger = spdlog::basic_logger_mt("File", filePath);
				}
				catch (const spdlog::spdlog_ex& e)
				{
					// An error occurred, print error message
					Log::error("[Log::enableFileLogging] Could not start file logger. Reason: {0}", e.what());
					s_fileLogger.reset();
				}
			}
			else
			{
				// Disable the file logging, reset pointer, etc
				s_fileLogger.reset();
				s_fileLogger = nullptr;
			}
		}
	}
}