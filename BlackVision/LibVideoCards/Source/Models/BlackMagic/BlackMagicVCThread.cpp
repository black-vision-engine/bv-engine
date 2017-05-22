#include "BlackMagicVCThread.h"
#include "BlackMagicVideoCard.h"
#include "UseLoggerVideoModule.h"

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
    , m_interlaceEnabled( false )
{
	for( SizeType i = 0; i < BUFFER_SIZE; ++i )
		m_prevFramesBuffer.push_back( MemoryChunk::Create( frameSize ) );
}


//**************************************
//
void                BlackMagicVCThread::EnableInterlacing       ( bool enable )
{
    m_interlaceEnabled = enable;
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
        auto biginFrameProcessingTime = m_videoCard->GetFrameTime();

        AVFrameConstPtr processedFrame = nullptr;

        if( m_interlaceEnabled )
            processedFrame = InterlaceFrame( srcFrame );
        else
        {
            processedFrame = AVFrame::Create( MemoryChunk::Create( srcFrame->m_videoData->Size() ), MemoryChunk::Create( srcFrame->m_audioData->Size() ), srcFrame->m_desc );

            memcpy( std::const_pointer_cast< MemoryChunk >( processedFrame->m_videoData )->GetWritable(), srcFrame->m_videoData->Get(), srcFrame->m_videoData->Size() );
            memcpy( std::const_pointer_cast< MemoryChunk >( processedFrame->m_audioData )->GetWritable(), srcFrame->m_audioData->Get(), srcFrame->m_audioData->Size() );
        }
            

        if( processedFrame )
            m_videoCard->FrameProcessed( processedFrame );

        if( m_frameDuration > 0 )
        {
            auto sleepFor = Int64( m_frameDuration ) - ( Int64( Time::Now() - biginFrameProcessingTime ) % Int64( m_frameDuration ) );
            // sleep to the next multiple of m_frameDuration.

            std::this_thread::sleep_for( std::chrono::milliseconds( sleepFor ) );
        }
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

    if( !m_prevAudioData || m_prevAudioData->Size() != frame->m_audioData->Size() * 2 )
    {
        m_prevAudioData = MemoryChunk::Create( frame->m_audioData->Size() * 2 );
    }

    if( !m_odd )
    {
        m_prevFrame = m_prevFramesBuffer.front();
        char * memDst = m_prevFrame->GetWritable();

        memcpy( memDst, memNew, size );

        memcpy( m_prevAudioData->GetWritable(), frame->m_audioData->Get(), frame->m_audioData->Size() );

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

        memcpy( m_prevAudioData->GetWritable() + frame->m_audioData->Size(), frame->m_audioData->Get(), frame->m_audioData->Size() );

        return AVFrame::Create( m_prevFrame, m_prevAudioData, frame->m_desc );
    }
}

} // blackmagic
} // videocards
} // bv