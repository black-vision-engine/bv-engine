#include "BlueFishInputThread.h"

#include "InputChannel.h"

#include "LibImage.h"


namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
BlueFishInputThread::BlueFishInputThread( InputChannel * vc )
    : m_processedFrameQueue( 1 )
    , m_inputChannel( vc )
{}

// ***********************
//
void                BlueFishInputThread::Process        ()
{
    auto cFrame = m_inputChannel->GetCaptureBuffer()->PopFrame();

    MemoryChunkPtr videoChunk = MemoryChunk::Create( cFrame->m_nSize );
    memcpy( videoChunk->GetWritable(), ( char * )cFrame->m_pBuffer, cFrame->m_nSize );

    MemoryChunkPtr audioChunk;// = MemoryChunk::Create( ( char * )cFrame->m_pAudioBuffer, cFrame->m_nAudioSize );

    auto processed = Deinterlace( videoChunk );
    auto frame = std::make_shared< AVFrame >( processed, audioChunk, m_inputChannel->CreateFrameDesc() );

    m_processedFrameQueue.WaitAndPush( frame );
}

// ***********************
//
MemoryChunkPtr      BlueFishInputThread::Deinterlace    ( MemoryChunkPtr videoChunk )
{
    // FIXME: Why we need to substract 4096. For some reason frame is to long.
    MemoryChunkPtr deinterlacedChunk = MemoryChunk::Create( 2 * videoChunk->Size() - 4096 );

    // Maybe we could choose algorithm here.
    DeinterlaceLinear( videoChunk, deinterlacedChunk );

    return deinterlacedChunk;
}

// ***********************
//
void                BlueFishInputThread::DeinterlaceLinear      ( MemoryChunkPtr inputChunk, MemoryChunkPtr outputChunk )
{
    auto desc = m_inputChannel->CreateFrameDesc();

    auto lineLength = desc.width * desc.depth;
    auto numLines = desc.height;

    // Copy each second line. Flip image vertically.
    for( SizeType lineNum = 0; lineNum < numLines / 2; ++lineNum )
    {
        const char * srcLine1Ptr = inputChunk->Get() + lineNum * lineLength;
        char * targetLine1Ptr = outputChunk->GetWritable() + ( numLines - 2 * lineNum - 1 ) * lineLength;

        memcpy( targetLine1Ptr, srcLine1Ptr, lineLength );
    }

    // Lacking line is computed as averaged 2 neighboring lines.
    for( SizeType lineNum = 1; lineNum < numLines - 2; lineNum += 2 )
    {
        const char * srcLine1Ptr = outputChunk->Get() + lineNum * lineLength;
        const char * srcLine2Ptr = outputChunk->Get() + ( lineNum + 2 ) * lineLength;

        char * targetMiddleLinePtr = outputChunk->GetWritable() + ( lineNum + 1 ) * lineLength;

        for( SizeType i = 0; i < lineLength; ++i )
        {
            int srcByte1 = ( unsigned char )srcLine1Ptr[ i ];
            int srcByte2 = ( unsigned char )srcLine2Ptr[ i ];

            targetMiddleLinePtr[ i ] = ( char )( ( srcByte1 + srcByte2 ) / 2 );
        }
    }

    // Fill first line.
    const char * srcLine1Ptr = outputChunk->Get() + lineLength;
    char * targetLine1Ptr = outputChunk->GetWritable();

    memcpy( targetLine1Ptr, srcLine1Ptr, lineLength );
}

//**************************************
//
void				BlueFishInputThread::EnqueueEndMessage		()
{
    m_processedFrameQueue.EnqueueEndMessage();
    m_processedFrameQueue.Clear();
}

// ***********************
//
AVFramePtr          BlueFishInputThread::PopNextFrame           ()
{
    AVFramePtr frame;
    m_processedFrameQueue.WaitAndPop( frame );
    return frame;
}


}   // bluefish
}   // videocards
}	// bv


