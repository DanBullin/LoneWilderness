/*! \file threadManager.cpp
*
* \brief A system which creates and manages threads
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/threadManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	bool ThreadManager::s_enabled = false; //!< Initialise with default value of false
	std::unordered_map<std::string, Thread*> ThreadManager::s_threads = std::unordered_map<std::string, Thread*>(); //!< Initialise empty list

	//! ThreadManager()
	ThreadManager::ThreadManager() : System(SystemType::ThreadManager)
	{
	}

	//! ~ThreadManager()
	ThreadManager::~ThreadManager()
	{
	}

	//! start()
	void ThreadManager::start()
	{
		// Start the system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[ThreadManager::start] Starting the thread manager system.");
			s_enabled = true;
		}
	}

	//! stop()
	void ThreadManager::stop()
	{
		// Stop the system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[ThreadManager::stop] Stopping the thread manager system.");

			// Delete all threads
			for (auto& thread : s_threads)
			{
				if (thread.second)
				{
					delete thread.second;
				}
			}
			s_threads.clear();

			s_enabled = false;
		}
	}

	//! checkThreadNameTaken()
	/*
	\param threadName a const std::string& - The name of the thread
	*/
	const bool ThreadManager::checkThreadNameTaken(const std::string& threadName)
	{
		return s_threads.find(threadName) != s_threads.end();
	}

	//! getThread()
	/*
	\param threadName a const std::stirng& - The name of the thread desired
	\return a Thread* - A pointer to the thread
	*/
	Thread* ThreadManager::getThread(const std::string& threadName)
	{
		if (checkThreadNameTaken(threadName))
			return s_threads[threadName];
		else
			return nullptr;
	}

	//! getThreads
	/*
	\return a std::unordered_map<std::string, Thread*>& - A reference to the list
	*/
	std::unordered_map<std::string, Thread*>& ThreadManager::getThreads()
	{
		return s_threads;
	}

	//! deleteThread()
	/*
	\param threadName a const std::string& - The name of the thread
	*/
	void ThreadManager::deleteThread(const std::string& threadName)
	{
		if (s_enabled)
		{
			if (checkThreadNameTaken(threadName))
			{
				if (s_threads[threadName])
				{
					delete s_threads[threadName];
				}
				s_threads.erase(threadName);
			}
			else
				ENGINE_ERROR("[ThreadManager::deleteThread] The thread with name: {0} cannot be found.", threadName);
		}
	}

	//! onUpdate()
	void ThreadManager::onUpdate(float timeStep, float totalTime)
	{
		if (s_enabled)
		{
			for (auto it = s_threads.cbegin(); it != s_threads.cend(); )
			{
				if (it->second)
				{
					if (it->second->isFinished())
					{
						if (!it->second->hasRejoined())
							it->second->rejoin();

						if (it->second->shouldDelete())
						{
							delete it->second;
							s_threads.erase(it++);
							continue;
						}
					}
				}
				++it;
			}
		}
	}

	//! printThreadDetails()
	void ThreadManager::printThreadsDetails()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("Thread Manager Details");
			ENGINE_TRACE("==========================");
			ENGINE_TRACE("Thread List Size: {0}", s_threads.size());
			ENGINE_TRACE("==========================");
		}
		else
			ENGINE_ERROR("[ThreadManager::printThreadsDetails] This system has not been enabled.");
	}

	//! printThreadDetails()
	/*
	\param threadName a const std::string& - The name of the thread
	*/
	void ThreadManager::printThreadDetails(const std::string& threadName)
	{
		if (s_enabled)
		{
			if (checkThreadNameTaken(threadName))
			{
				if (s_threads[threadName])
					s_threads[threadName]->printDetails();
			}
			else
				ENGINE_ERROR("[ThreadManager::printThreadDetails] The thread cannot be found. Name: {0}.", threadName);
		}
		else
			ENGINE_ERROR("[ThreadManager::printThreadDetails] This system has not been enabled.");
	}
}