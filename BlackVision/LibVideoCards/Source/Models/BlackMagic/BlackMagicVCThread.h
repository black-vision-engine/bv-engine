#pragma once 

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Threading/StoppableThread.h"

#include "DataTypes/QueueConcurrentLimited.h"

#include "AVFrame.h"

namespace bv { namespace videocards { namespace blackmagic 
{

class BlackMagicVCThread : public StoppableThread
{
	typedef std::shared_ptr< QueueConcurrentLimited< AVFramePtr > >    FrameQueue;

	FrameQueue					m_frameQueue;

public:
	explicit					BlackMagicVCThread		();

	virtual void				Process					() override;
};

DEFINE_UPTR_TYPE( BlackMagicVCThread )

} // blackmagic
} // videocards
} // bv
