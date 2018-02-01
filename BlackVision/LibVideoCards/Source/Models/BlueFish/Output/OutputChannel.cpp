#include "OutputChannel.h"


#include "BlueFishVCThread.h"



namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
OutputChannel::OutputChannel( ChannelName name, ChannelOutputDataUPtr & output )
    : Channel( name )
    , PlaybackData( std::move( output ) )
    , m_playbackChannel( nullptr )
    , m_playbackFifoBuffer( nullptr )
    , m_frameProcessingThread( nullptr )
{
    m_playbackFifoBuffer = new CFifoBuffer();
    m_playbackChannel = new CFifoPlayback();

    m_frameProcessingThread = new BlueFishVCThread( this, 1920 * 1080 * 4 ); // FIXME: Set frame size properly.

    if( PlaybackData->interlaced )
    {
        m_frameProcessingThread->EnableInterlacing( true );
        //m_frameProcessingThread->SetFrameDuration( UInt64( 1000 / ( float(output->refresh) / 100.f ) ) );
    }

    m_frameProcessingThread->Start();
}

// ***********************
//
OutputChannel::~OutputChannel()
{
    delete m_playbackChannel;
    m_playbackChannel = nullptr;

    m_frameProcessingThread->Kill();
    m_frameProcessingThread->EnqueueEndMessage();
    m_frameProcessingThread->Join();

    delete m_frameProcessingThread;
    m_frameProcessingThread = nullptr;
}

//**************************************
//
CFifoBuffer *           OutputChannel::GetPlaybackBuffer        ()
{
    return m_playbackFifoBuffer;
}

//**************************************
//
CFifoPlayback *         OutputChannel::GetPlaybackChannel       () const
{
    return m_playbackChannel;
}

//**************************************
//
IOType                  OutputChannel::GetOutputType            () const
{
    return PlaybackData->type;
}

// ***********************
//
UInt64                  OutputChannel::GetOutputId              () const
{
    return PlaybackData->linkedVideoOutput;
}

// ***********************
//
UInt32                  OutputChannel::GetOutputChannel         () const
{
    return ChannelOptions.at( m_channelName ).OutputChannel;
}

// ***********************
//
UInt32                  OutputChannel::GetEpochSDIOutput        () const
{
    return ChannelOptions.at( m_channelName ).EpochSDIOutput;
}

// ***********************
//
UInt32                  OutputChannel::GetEpochOutputMemInterface   () const
{
    return ChannelOptions.at( m_channelName ).EpochOutputMemInterface;
}

// ***********************
//
UInt32                  OutputChannel::GetEpochSDIKeyOutput         () const
{
    return ChannelOptions.at( m_channelName ).EpochSDIKeyOutput;
}

//**************************************
//
UInt32                  OutputChannel::GetReferenceMode             () const
{
    return ( UInt32 )PlaybackData->referenceMode;
}

//**************************************
//
UInt32                  OutputChannel::GetReferenceH                () const
{
    return ( UInt32 )PlaybackData->referenceH;
}

//**************************************
//
UInt32                  OutputChannel::GetReferenceV                () const
{
    return ( UInt32 )PlaybackData->referenceV;
}

//**************************************
//
bool                    OutputChannel::GetFlipped                   () const
{
    return PlaybackData->flipped;
}

// ***********************
//
UInt32                  OutputChannel::GetVideoMode          () const
{
    return PlaybackData->videoMode;
}

// ***********************
//
UInt32                  OutputChannel::GetUpdateFormat       () const
{
    return PlaybackData->updateFormat;
}

// ***********************
//
UInt32                  OutputChannel::GetMemoryFormat       () const
{
    return PlaybackData->memoryFormat;
}

//**************************************
//
bool                    OutputChannel::AutoGeneratedTimecode  () const
{
    return PlaybackData->autoGeneratedTimecode;
}

//**************************************
//
bool                    OutputChannel::TimecodePresented            () const
{
    return PlaybackData->timecodePresented;
}


// ***********************
//
void                    OutputChannel::SetVideoOutput               ( bool enable )
{
    if( GetPlaybackChannel() && GetPlaybackChannel()->m_pSDK )
    {
        VARIANT value;
        value.vt = VT_UI4;
        value.ulVal = enable ? ENUM_BLACKGENERATOR_OFF : ENUM_BLACKGENERATOR_ON;
        GetPlaybackChannel()->m_pSDK->SetCardProperty( VIDEO_BLACKGENERATOR, value );
    }
}

// ***********************
//
UInt32                  OutputChannel::GetResolution                () const
{
    return PlaybackData->resolution;
}

// ***********************
//
void                    OutputChannel::StartThreads                 ()
{
    m_playbackChannel->StartThread();
}

// ***********************
//
void                    OutputChannel::StopThreads                  ()
{
    m_playbackChannel->StopThread();
}

// ***********************
//
void                    OutputChannel::SuspendThreads               ()
{
    m_playbackChannel->SuspendThread();
}

// ***********************
//
void                    OutputChannel::ResumeThreads                ()
{
    m_playbackChannel->StartThread();
}

// ***********************
//
bool                    OutputChannel::IsInputChannel               () const
{
    return false;
}

// ***********************
//
bool                    OutputChannel::IsOutputChannel              () const
{
    return true;
}

void                    OutputChannel::EnqueueFrame                 ( const AVFrameConstPtr & frame )
{
    m_frameProcessingThread->EnqueueFrame( frame );
}

// ***********************
//
void                    OutputChannel::FrameProcessed               ( const AVFrameConstPtr & frame )
{
    auto playbackChannel = GetPlaybackChannel();
    if( playbackChannel )
    {
        BVTimeCode tc; // FIXME: https://www.pivotaltracker.com/story/show/145508031
        tc.h = 10;
        tc.m = 22;
        tc.s = 33;
        tc.frame = 12;

        auto cFrame = playbackChannel->m_reusableFrames.GetNext();

        cFrame->ReinitVideoBuffer( playbackChannel->GoldenSize, playbackChannel->BytesPerLine, reinterpret_cast< const unsigned char * >( frame->m_videoData->Get() ) );
        cFrame->ReinitAudioBuffer( frame->m_audioData ? ( unsigned int )frame->m_audioData->Size() : 0, frame->m_audioData ? reinterpret_cast< const unsigned char * >( frame->m_audioData->Get() ) : nullptr );

        cFrame->m_FieldOdd = !m_odd;
        cFrame->m_desc = frame->m_desc;
        cFrame->m_TimeCode = tc;

        playbackChannel->m_pFifoBuffer->PushFrame( cFrame );

        m_odd = ( m_odd + 1 ) % 2;
    }
}


//**************************************
//
void                    OutputChannel::UpdateFrameTime      ( UInt64 t )
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_lastFrameTime = t;
}

//**************************************
//
UInt64                  OutputChannel::GetFrameTime         () const
{
    std::unique_lock< std::mutex > lock( m_mutex );
    return m_lastFrameTime;
}




} //bluefish
} //videocards
} //bv



