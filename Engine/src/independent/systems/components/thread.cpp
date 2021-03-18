/*! \file thread.cpp
*
* \brief A class which is a wrapper for std::thread and is managed by the thread manager
*
* \author Daniel Bullin
*
*/
#include "independent/systems/components/thread.h"

namespace Engine
{
	//! Thread
	/*
	\param threadName a const std::string& - The name of the thread
	\param deleteUponFinished a const bool - Delete the thread upon completion
	*/
	Thread::Thread(const std::string& threadName, const bool deleteUponFinished) : m_threadName(threadName), m_isFinished(false), m_deleteUponFinish(deleteUponFinished), m_rejoined(false)
	{
		ENGINE_INFO("[Thread::Thread] Creating thread: {0}.", m_threadName);
	}

	//! ~Thread()
	Thread::~Thread()
	{
		ENGINE_INFO("[Thread::~Thread] Deleting thread: {0}.", m_threadName);
	}

	//! getName()
	/*
	\return a const std::string& - The name of the thread
	*/
	const std::string& Thread::getName()
	{
		return m_threadName;
	}

	//! isFinished()
	/*
	\return a const bool - Has the thread finished
	*/
	const bool Thread::isFinished()
	{
		return m_isFinished;
	}

	//! shouldDelete()
	/*
	\return a const bool - Should the thread be deleted upon being finished
	*/
	const bool Thread::shouldDelete()
	{
		return m_deleteUponFinish;
	}

	//! hasRejoined()
	/*
	\return a const bool - Has the thread rejoined
	*/
	const bool Thread::hasRejoined()
	{
		return m_rejoined;
	}

	//! rejoin()
	void Thread::rejoin()
	{
		if (m_isFinished && !m_rejoined)
		{
			ENGINE_INFO("[Thread::rejoin] {0} has been rejoined.", m_threadName);
			m_thread.join();
			m_rejoined = true;
		}
		else
			ENGINE_ERROR("[Thread::rejoin] The thread {0} hasn't finished executing.", m_threadName);
	}

	//! printDetails()
	void Thread::printDetails()
	{
		ENGINE_TRACE("===========================================");
		ENGINE_TRACE("Thread Address: {0}", (void*)this);
		ENGINE_TRACE("Thread Name: {0}", getName());
		ENGINE_TRACE("Has Finished: {0}", m_isFinished);
		ENGINE_TRACE("Delete Upon Finish: {0}", m_deleteUponFinish);
		ENGINE_TRACE("Has Rejoined: {0}", m_rejoined);
		ENGINE_TRACE("===========================================");
	}
}