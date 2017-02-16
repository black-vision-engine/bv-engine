#pragma once 

#include <atomic>
#include <mutex>
#include <condition_variable>
#pragma once

#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrentLimited.h"

#include "AVFrame.h"


namespace bv { namespace videocards { namespace blackmagic 
{

class VideoCard;

class BlackMagicVCThread : public StoppableThread
{
	typedef QueueConcurrentLimited< AVFramePtr >    FrameQueue;

	FrameQueue					m_frameQueue;

	MemoryChunkPtr				m_prevFrame;

	VideoCard *					m_videoCard;

	void						InterlaceFrame			( AVFramePtr frame );

public:
	explicit					BlackMagicVCThread		( VideoCard * vc );

	void						EnqueueFrame			( const AVFramePtr & frame );

	void						EnqueueEndMessage		();

	virtual void				Process					() override;
};

DEFINE_UPTR_TYPE( BlackMagicVCThread )

} // blackmagic
} // videocards
} // bv
