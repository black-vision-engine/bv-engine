#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "CoreDEF.h"

#include "Threading/Thread.h"
#include "VideoCardManager.h"

namespace bv { namespace videocards {


class ProcessingThread : public Thread
{

private:

	mutable std::mutex			m_mutex;
	std::condition_variable		m_cond;

	std::atomic< bool >			m_running;

    VideoCardManager *          m_videoCardManager;

public:
								ProcessingThread	( VideoCardManager * videoCardManager );
	virtual						~ProcessingThread	();
	void						Kill				();

protected:

	virtual void				Run					() override;

};

DEFINE_UPTR_TYPE( ProcessingThread )

} //videocards
} //bv
