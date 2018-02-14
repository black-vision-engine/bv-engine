#include "BlueFishInputThread.h"

#include "InputChannel.h"

#include "LibImage.h"

#include "UseLoggerVideoModule.h"


namespace bv {
namespace videocards {
namespace bluefish
{


const int FramesQueueSize = 2;


// ***********************
//
BlueFishInputThread::BlueFishInputThread( InputChannel * vc )
    : m_processedFrameQueue( FramesQueueSize )
    , m_inputChannel( vc )
    , m_reusableChunks( { nullptr } )
{
    m_reusableChunks = m_inputChannel->CreateReusableChunks( 2 * FramesQueueSize );

    // Add one frame delay to avoid waiting for input in main thread.
    auto emptyFrame = GenEmptyFrame();
    bool success = m_processedFrameQueue.TryPush( emptyFrame );

    // If push failed, something is horibly wrong.
    assert( success );
    if( !success )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "[BlueFishInputThread] Can't push first frame to video input queue";
    }
}

// ***********************
//
void                BlueFishInputThread::Process        ()
{
    auto cFrame = m_inputChannel->GetCaptureBuffer()->PopFrame();

    MemoryChunkPtr audioChunk;// = MemoryChunk::Create( ( char * )cFrame->m_pAudioBuffer, cFrame->m_nAudioSize );

    auto processed = Deinterlace( cFrame );
    auto frame = std::make_shared< AVFrame >( processed, audioChunk, m_inputChannel->CreateFrameDesc() );

    m_processedFrameQueue.WaitAndPush( frame );
}

// ***********************
//
MemoryChunkPtr      BlueFishInputThread::Deinterlace            ( const CFramePtr & videoChunk )
{
    MemoryChunkPtr deinterlacedChunk = m_reusableChunks.GetNext();
    deinterlacedChunk->Clear();

    // Maybe we could choose algorithm here.
    DeinterlaceLinear( videoChunk, deinterlacedChunk );

    return deinterlacedChunk;
}

// ***********************
//
void                BlueFishInputThread::DeinterlaceLinear      ( const CFramePtr & inputChunk, MemoryChunkPtr outputChunk )
{
    auto desc = m_inputChannel->CreateFrameDesc();

    auto lineLength = desc.width * desc.depth;
    auto numLines = desc.height;

    // Copy each second line. Flip image vertically.
    for( SizeType lineNum = 0; lineNum < numLines / 2; ++lineNum )
    {
        const char * srcLine1Ptr = reinterpret_cast< const char* >( inputChunk->m_pBuffer ) + lineNum * lineLength;
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
    m_processedFrameQueue.TryPop( frame );
    return frame;
}


// ***********************
//
AVFramePtr          BlueFishInputThread::GenEmptyFrame          ()
{
    auto desc = m_inputChannel->CreateFrameDesc();

    auto chunkSize = desc.depth * desc.height * desc.width;

    auto videoChunk = MemoryChunk::Create( chunkSize );
    MemoryChunkPtr audioChunk = nullptr;

    return std::make_shared< AVFrame >( videoChunk, audioChunk, desc );
}

}   // bluefish
}   // videocards
}	// bv


