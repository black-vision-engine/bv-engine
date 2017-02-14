#include "BlackMagicVCThread.h"

namespace bv { namespace videocards { namespace blackmagic 
{

//**************************************
//
BlackMagicVCThread::BlackMagicVCThread							( IDeckLinkOutput * output )
	: m_output( output )
	, m_decklinkFrame( nullptr )
	, m_frameQueue( 1 )
{}

//**************************************
//
void				BlackMagicVCThread::SetDecklinkFrame		( IDeckLinkMutableVideoFrame * frame )
{
	m_decklinkFrame = frame;
}

//**************************************
//
void				BlackMagicVCThread::EnqueueFrame			( const AVFramePtr & frame )
{
	m_frameQueue.WaitAndPush( frame );
}

//**************************************
//
void				BlackMagicVCThread::Process					()
{
	AVFramePtr srcFrame;

	if( m_frameQueue.WaitAndPop( srcFrame ) )
	{
		void * rawFrame;
		m_decklinkFrame->GetBytes( &rawFrame );

		memcpy( rawFrame, srcFrame->m_videoData->Get(), m_decklinkFrame->GetRowBytes() * m_decklinkFrame->GetHeight() );

		m_output->DisplayVideoFrameSync( m_decklinkFrame );
	}
}

} // blackmagic
} // videocards
} // bv