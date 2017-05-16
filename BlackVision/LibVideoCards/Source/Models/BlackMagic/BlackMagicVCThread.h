#pragma once 

#include <atomic>
#include <mutex>
#include <condition_variable>
#pragma once

#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrentLimited.h"

#include "Memory/AVFrame.h"

#include <boost/circular_buffer.hpp>

namespace bv { namespace videocards { namespace blackmagic 
{

class VideoCard;

class BlackMagicVCThread : public StoppableThread
{
	typedef QueueConcurrentLimited< AVFrameConstPtr >    FrameQueue;

	boost::circular_buffer< MemoryChunkPtr > m_prevFramesBuffer;

	MemoryChunkPtr				m_prevFrame;

    MemoryChunkPtr              m_prevAudioData;

	FrameQueue					m_frameQueue;

	bool						m_odd;
    bool						m_interlaceEnabled;

    UInt64                      m_frameDuration;

	VideoCard *					m_videoCard;

	AVFrameConstPtr				InterlaceFrame			( const AVFrameConstPtr & frame );

public:
	explicit					BlackMagicVCThread		( VideoCard * vc, SizeType frameSize );

    void                        SetFrameDuration        ( UInt64 frameDuration );
    void                        EnableInterlacing       ( bool enable );

	void						EnqueueFrame			( const AVFrameConstPtr & frame );

	void						EnqueueEndMessage		();

	virtual void				Process					() override;
};

DEFINE_UPTR_TYPE( BlackMagicVCThread )

} // blackmagic
} // videocards
} // bv
