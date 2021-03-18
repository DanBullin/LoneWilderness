/*! \file thread.h
*
* \brief A class which is a wrapper for std::thread and is managed by the thread manager
*
* \author Daniel Bullin
*
*/
#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include "independent/systems/systems/log.h"

namespace Engine
{
	/*! \class Thread
	* \brief A class which is a wrapper for std::thread
	*/
	class Thread
	{
	private:
		std::string m_threadName; //!< The name of the thread
		std::thread m_thread; //!< The std thread
		bool m_isFinished; //!< Has the thread finished executing
		bool m_deleteUponFinish; //!< Delete the thread upon completion
		bool m_rejoined; //!< Has the std::thread rejoined
	public:
		Thread(const std::string& threadName, const bool deleteUponFinished); //!< Constructor
		~Thread(); //!< Destructor

		template<typename Function, typename ...Args> void action(Function&& func, Args&&... args); //!< Start the function

		const std::string& getName(); //!< Get the name of the thread
		const bool isFinished(); //!< Has the thread finished executing
		const bool shouldDelete(); //!< Should the thread be deleted upon being finished
		const bool hasRejoined(); //!< Has the std::thread rejoined
		void rejoin(); //!< Rejoin the std::thread to the main thread

		void printDetails(); //!< Print the thread's details
	};

	template<typename Function, typename... Args>
	//! action()
	/*
	\param func a Function&& - The function to run
	\param args an Args&& - The parameter pack for the function
	*/
	void Thread::action(Function&& func, Args&&... args)
	{
		// Execute the function then set the finished bool to true
		m_isFinished = false;

		m_thread = std::thread([&]()
		{
			func(std::forward<Args>(args)...);
			m_isFinished = true;
			ENGINE_INFO("[Thread::action] Finished executing thread: {0}.", m_threadName);
		});
	}
}
#endif