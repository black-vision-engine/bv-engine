#pragma once 

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Threading/Thread.h"


namespace bv
{

class StoppableThread : public Thread
{
	bool						m_running;

	bool						m_stopped;
	bool						m_stop;

	std::condition_variable		m_waitResumed;
	std::condition_variable		m_waitStopped; 

	std::mutex					m_mutex;

public:
	explicit					StoppableThread			();

	virtual void				Run				    	() override;

	virtual void				Process					() = 0;

	void						Stop					();
	void						WaitUntilStopped		();
	void						Resume					();

	void						Kill					();
};

} // bv
