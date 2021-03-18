/*! \file threadManager.h
*
* \brief A system which creates and manages threads
*
* \author Daniel Bullin
*
*/
#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "independent/systems/system.h"
#include "independent/systems/components/thread.h"

namespace Engine
{
	/*! \class ThreadManager
	* \brief A system which creates and manages threads
	*/
	class ThreadManager : public System
	{
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::unordered_map<std::string, Thread*> s_threads; //!< A list of all created threads
	public:
		ThreadManager(); //!< Constructor
		~ThreadManager(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static const bool checkThreadNameTaken(const std::string& threadName); //!< Check if a thread name is taken
		template<typename Function, typename ...Args> static void startThread(const std::string& threadName, const bool deleteOnceFinished, Function&& func, Args&&... args); //!< Create a thread
		static Thread* getThread(const std::string& threadName); //!< Get a thread by name
		static std::unordered_map<std::string, Thread*>& getThreads(); //!< Get a list of all current threads
		static void deleteThread(const std::string& threadName); //!< Delete a thread after its finished executing
		static void onUpdate(float timeStep, float totalTime); //!< Update the thread manager

		static void printThreadsDetails(); //!< Print thread manager details
		static void printThreadDetails(const std::string& threadName); //!< Print a thread's details
	};

	template<typename Function, typename... Args>
	//! startThread()
	/*
	\param threadName a const std::string& - The name of the thread
	\param deleteOnceFinished a const bool - Delete the thread upon completion
	\param func a Function&& - The function to run on a different thread
	\param args an Args&& - The arguments for the function
	*/
	void ThreadManager::startThread(const std::string& threadName, const bool deleteOnceFinished, Function&& func, Args&&... args)
	{
		if (!checkThreadNameTaken(threadName) && threadName != "")
		{
			Thread* thread = new Thread(threadName, deleteOnceFinished);
			if (thread)
			{
				s_threads[threadName] = thread;
				thread->action(func, std::forward<Args>(args)...);
			}
		}
		else
			ENGINE_INFO("[ThreadManager::startThread] Thread name already taken. Name: {0}.", threadName);
	}
}
#endif 
