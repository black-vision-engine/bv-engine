#include "BlueFishVCThread.h"

#include "Models/BlueFish/BlueFishVideoCard.h"
#include "UseLoggerVideoModule.h"

#include "System/Time.h"
#include "Serialization/ConversionHelper.h"

#include "UseLoggerVideoModule.h"



namespace bv { namespace videocards { namespace bluefish
{

namespace
{
const SizeType      BUFFER_SIZE = 10;
}



//**************************************
//
BlueFishVCThread::BlueFishVCThread							( OutputChannel * vc, SizeType frameSize )
    : m_frameQueue( 1 )
    , m_videoChannel( static_cast< OutputChannel * >( vc ) )
    , m_odd( false )
    , m_outputFramesBuffer( BUFFER_SIZE )
    , m_frameDuration( 20 )     // 20 ms 50 Hz by default
    , m_interlaceEnabled( false )
{
    frameSize = frameSize / ( m_videoChannel->PlaybackData->interlaced ? 2 : 1 ) + 2048; // FIXME: Why + 2048

	for( SizeType i = 0; i < BUFFER_SIZE; ++i )
        m_outputFramesBuffer.push_back( MemoryChunk::Create( frameSize ) );

    auto desc = vc->CreateFrameDesc();
    auto audioSize = desc.channelDepth * desc.channels * desc.numSamples;

    m_prevAudioData = MemoryChunk::Create( audioSize );
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
        AVFrameConstPtr processedFrame = nullptr;

        if( m_interlaceEnabled )
            processedFrame = InterlaceFrame( srcFrame );

		if( processedFrame )
            m_videoChannel->FrameProcessed( processedFrame );
        else
            m_videoChannel->FrameProcessed( srcFrame );

        
        m_videoChannel->UpdateFrameTime( Time::Now() );
	}
}

// *********************************
//
AVFrameConstPtr		BlueFishVCThread::InterlaceFrame( const AVFrameConstPtr & frame )
{		
    m_odd = !m_odd;

    const char *memSrc = frame->m_videoData->Get();

    int pixel_depth = frame->m_desc.depth;  // pobra? poni?sze informacje (wdepth,  width, height z configa, albo niech tu nie przychodzi RawData tylko jako? to opakowane w klas? typu Frame
    int width = frame->m_desc.width;
    int height = frame->m_desc.height;
    int bytes_per_line = width * pixel_depth;

    auto outputFrame = m_outputFramesBuffer.front();
    m_outputFramesBuffer.pop_front();
    m_outputFramesBuffer.push_back( outputFrame );

    char * memDst = outputFrame->GetWritable();  // pewnie nie ma co tutaj tego za kazdym razem tworzy?...

    auto size = outputFrame->Size();

    memset( memDst, 0, size );

    for( int i = m_odd, j = 0; i < height; i += 2, j++ )
        memcpy( &memDst[ j*( bytes_per_line ) ], &memSrc[ i*( bytes_per_line ) ], bytes_per_line );

    MemoryChunkPtr audioData = nullptr;
    AVFrameDescriptor newDesc = frame->m_desc;

    if( !m_prevAudioData )
    {
        m_prevAudioData = MemoryChunk::Create( frame->m_audioData->Size() );
        memcpy( m_prevAudioData->GetWritable(), frame->m_audioData->Get(), frame->m_audioData->Size() );

        newDesc.numSamples = 0;
    }
    else
    {
        audioData = MemoryChunk::Create( frame->m_audioData->Size() + m_prevAudioData->Size() );
        memcpy( audioData->GetWritable(), m_prevAudioData->Get(), m_prevAudioData->Size() );
        memcpy( audioData->GetWritable() + m_prevAudioData->Size(), frame->m_audioData->Get(), frame->m_audioData->Size() );
        newDesc.numSamples = 2 * frame->m_desc.numSamples;
        
        m_prevAudioData = nullptr;
    }

    return AVFrame::Create( outputFrame, audioData, newDesc );
}

} // blackmagic
} // videocards
} // bv