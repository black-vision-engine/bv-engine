#include "BlueFishVCThread.h"
#include "BlueFishVideoCard.h"
#include "UseLoggerVideoModule.h"

#include "System/Time.h"

namespace bv { namespace videocards { namespace bluefish
{

namespace
{
const SizeType      BUFFER_SIZE = 10;
}

//**************************************
//
BlueFishVCThread::BlueFishVCThread							( Channel * vc, SizeType frameSize )
    : m_frameQueue( 1 )
    , m_videoChannel( vc )
    , m_odd( true )
    , m_outputFramesBuffer( BUFFER_SIZE )
    , m_frameDuration( 0 )
    , m_interlaceEnabled( false )
{
    frameSize = frameSize / ( vc->m_playbackData->interlaced ? 2 : 1 ) + 2048; // FIXME: Why + 2048

	for( SizeType i = 0; i < BUFFER_SIZE; ++i )
        m_outputFramesBuffer.push_back( MemoryChunk::Create( frameSize ) );
}


//**************************************
//
void                BlueFishVCThread::EnableInterlacing       ( bool enable )
{
    m_interlaceEnabled = enable;
}

//**************************************
//
void                BlueFishVCThread::SetFrameDuration        ( UInt64 frameDuration )
{
    m_frameDuration = frameDuration;
}

//**************************************
//
void				BlueFishVCThread::EnqueueFrame			( const AVFrameConstPtr & frame )
{
	m_frameQueue.WaitAndPush( frame );
}

//**************************************
//
void				BlueFishVCThread::EnqueueEndMessage		()
{
	m_frameQueue.EnqueueEndMessage();
}

//**************************************
//
void				BlueFishVCThread::Process					()
{
	AVFrameConstPtr srcFrame;

	if( m_frameQueue.WaitAndPop( srcFrame ) )
	{
        auto biginFrameProcessingTime = m_videoChannel->GetFrameTime();

        AVFrameConstPtr processedFrame = nullptr;

        if( m_interlaceEnabled )
            processedFrame = InterlaceFrame( srcFrame );

		if( processedFrame )
            m_videoChannel->FrameProcessed( processedFrame );
        else
            m_videoChannel->FrameProcessed( srcFrame );

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
AVFrameConstPtr		BlueFishVCThread::InterlaceFrame( const AVFrameConstPtr & frame )
{		
	m_odd = !m_odd;

    const char *memSrc = frame->m_videoData->Get();

    int pixel_depth = frame->m_desc.depth;  // pobraæ poni¿sze informacje (wdepth,  width, height z configa, albo niech tu nie przychodzi RawData tylko jakoœ to opakowane w klasê typu Frame
    int width = frame->m_desc.width;
    int height = frame->m_desc.height;
    int bytes_per_line = width * pixel_depth;

    auto outputFrame = m_outputFramesBuffer.front();

    char * memDst = outputFrame->GetWritable();  // pewnie nie ma co tutaj tego za kazdym razem tworzyæ...

    auto size = outputFrame->Size();

    memset( memDst, 0, size );

    for( int i = m_odd, j = 0; i < height; i += 2, j++ )
    {
        memcpy( &memDst[ j*( bytes_per_line ) ], &memSrc[ i*( bytes_per_line ) ], bytes_per_line );
    }

    return AVFrame::Create( outputFrame, frame->m_audioData, frame->m_desc );
}

} // blackmagic
} // videocards
} // bv