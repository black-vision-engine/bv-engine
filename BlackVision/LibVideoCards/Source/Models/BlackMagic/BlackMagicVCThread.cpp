#include "BlackMagicVCThread.h"
#include "BlackMagicVideoCard.h"
#include "System/Time.h"

namespace bv { namespace videocards { namespace blackmagic 
{

namespace
{
const SizeType      BUFFER_SIZE = 10;
}

//**************************************
//
BlackMagicVCThread::BlackMagicVCThread							( VideoCard * vc, SizeType frameSize )
    : m_frameQueue( 1 )
    , m_videoCard( vc )
    , m_odd( true )
    , m_prevFramesBuffer( BUFFER_SIZE )
    , m_frameDuration( 0 )
{
	for( SizeType i = 0; i < BUFFER_SIZE; ++i )
		m_prevFramesBuffer.push_back( MemoryChunk::Create( frameSize ) );
}

//**************************************
//
void                BlackMagicVCThread::SetFrameDuration        ( UInt64 frameDuration )
{
    m_frameDuration = frameDuration;
}

//**************************************
//
void				BlackMagicVCThread::EnqueueFrame			( const AVFrameConstPtr & frame )
{
	m_frameQueue.WaitAndPush( frame );
}

//**************************************
//
void				BlackMagicVCThread::EnqueueEndMessage		()
{
	m_frameQueue.EnqueueEndMessage();
}

//**************************************
//
void				BlackMagicVCThread::Process					()
{
	AVFrameConstPtr srcFrame;

	if( m_frameQueue.WaitAndPop( srcFrame ) )
	{
        auto biginFrameProcessingTime = Time::Now();

		auto data = InterlaceFrame( srcFrame );

		if( data )
			m_videoCard->FrameProcessed( data );

        auto sleepFor = m_frameDuration - ( Time::Now() - biginFrameProcessingTime );
        std::this_thread::sleep_for( std::chrono::milliseconds( sleepFor ) );
	}
}

// *********************************
//
AVFrameConstPtr		BlackMagicVCThread::InterlaceFrame( const AVFrameConstPtr & frame )
{
	int pixel_depth = frame->m_desc.depth;
	int width = frame->m_desc.width;
	int height = frame->m_desc.height;
	int bytes_per_line = width * pixel_depth;
	int size = width * height * pixel_depth;
	
	const char * memNew = frame->m_videoData->Get();
		
	m_odd = !m_odd;

	if( !m_odd )
	{
		m_prevFrame = m_prevFramesBuffer.front();
		char * memDst = m_prevFrame->GetWritable();

		memcpy( memDst, memNew, size );
		return nullptr;
	}
	else
	{
		char * memDst = m_prevFrame->GetWritable();

		for( int i = 0; i < height; i++ )
		{
			if( i % 2 != 1 )
				memcpy( &memDst[ i*( bytes_per_line ) ], &memNew[ i*( bytes_per_line ) ], bytes_per_line );
		}

		m_prevFramesBuffer.push_back( m_prevFrame );

		return AVFrame::Create( m_prevFrame, frame->m_audioData, frame->m_desc );
	}
}

} // blackmagic
} // videocards
} // bv