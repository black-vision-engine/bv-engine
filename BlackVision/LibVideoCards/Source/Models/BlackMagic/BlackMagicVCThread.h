#pragma once 

#include <atomic>
#include <mutex>
#include <condition_variable>
#pragma once

#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrentLimited.h"

#include "AVFrame.h"
#include "BlackMagicUtils.h"

#include "BlackMagic/DeckLinkAPI_h.h"


namespace bv { namespace videocards { namespace blackmagic 
{

class BlackMagicVCThread : public StoppableThread
{
	typedef QueueConcurrentLimited< AVFramePtr >    FrameQueue;

	FrameQueue						m_frameQueue;

	IDeckLinkOutput *               m_output;
	IDeckLinkMutableVideoFrame *	m_decklinkFrame;

public:
	explicit					BlackMagicVCThread		( IDeckLinkOutput * output );

	void						SetDecklinkFrame		( IDeckLinkMutableVideoFrame * frame );

	void						EnqueueFrame			( const AVFramePtr & frame );

	void						EnqueueEndMessage		();

	virtual void				Process					() override;
};

DEFINE_UPTR_TYPE( BlackMagicVCThread )

} // blackmagic
} // videocards
} // bv
