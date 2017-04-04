#pragma once 

#include <atomic>
#include <mutex>
#include <condition_variable>
#pragma once

#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrentLimited.h"

#include "AVFrame.h"
#include "Channel.h"

#include <boost/circular_buffer.hpp>

namespace bv { namespace videocards { namespace bluefish 
{

class VideoCard;

class BlueFishVCThread : public StoppableThread
{
	typedef QueueConcurrentLimited< AVFrameConstPtr >    FrameQueue;

	boost::circular_buffer< MemoryChunkPtr > m_outputFramesBuffer;

	FrameQueue					m_frameQueue;

	bool						m_odd;
    bool						m_interlaceEnabled;

    UInt64                      m_frameDuration;

	Channel *					m_videoChannel;

	AVFrameConstPtr				InterlaceFrame			( const AVFrameConstPtr & frame );

public:
	explicit					BlueFishVCThread		( Channel * vc, SizeType frameSize );

    void                        SetFrameDuration        ( UInt64 frameDuration );
    void                        EnableInterlacing       ( bool enable );

	void						EnqueueFrame			( const AVFrameConstPtr & frame );

	void						EnqueueEndMessage		();

	virtual void				Process					() override;
};

DEFINE_UPTR_TYPE( BlueFishVCThread )

} // bluefish
} // videocards
} // bv
