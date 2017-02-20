#pragma once 

#include <atomic>
#include <mutex>
#include <condition_variable>
#pragma once

#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrentLimited.h"

#include "AVFrame.h"

#include <boost/circular_buffer.hpp>

namespace bv { namespace videocards { namespace blackmagic 
{

class VideoCard;

class BlackMagicVCThread : public StoppableThread
{
	typedef QueueConcurrentLimited< AVFrameConstPtr >    FrameQueue;

	boost::circular_buffer< MemoryChunkPtr > m_prevFramesBuffer;

	MemoryChunkPtr				m_prevFrame;

	FrameQueue					m_frameQueue;

	bool						m_odd;

	VideoCard *					m_videoCard;

	AVFrameConstPtr				InterlaceFrame			( const AVFrameConstPtr & frame );

public:
	explicit					BlackMagicVCThread		( VideoCard * vc, SizeType frameSize );

	void						EnqueueFrame			( const AVFrameConstPtr & frame );

	void						EnqueueEndMessage		();

	virtual void				Process					() override;
};

DEFINE_UPTR_TYPE( BlackMagicVCThread )

} // blackmagic
} // videocards
} // bv
