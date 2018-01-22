#include "Channel.h"

#include "System/Time.h"

#include "BlueFishVCThread.h"

#include <process.h>


namespace bv { namespace videocards { namespace bluefish {




// ***********************
/// @todo Add SD resolution.
UInt32          Resolution2Width    ( UInt32 resolutionWidth )
{
    switch( resolutionWidth )
    {
    case 1080:
        return 1920;
    case 720:
        return 1280;
    }

    return 1920;
}






//**************************************
//
Channel::ChannelOptionMap     Channel::ChannelOptions = CreateChannelOptionMap();

//**************************************
//
Channel::Channel( ChannelName name, ChannelInputDataUPtr & input, ChannelOutputDataUPtr & output )
    : m_channelName( name )
    , m_captureData( nullptr )
    , m_captureChannel( nullptr )
    , m_captureFifoBuffer( nullptr )
    , m_playbackData( nullptr )
    , m_playbackChannel( nullptr )
    , m_playbackFifoBuffer( nullptr )
    , m_frameProcessingThread( nullptr )
    , m_inputFramesThread( nullptr )
    , m_odd( 0 )
{
    if( input ) 
    {
        m_captureData = std::move( input );
        m_captureFifoBuffer = new CFifoBuffer();
        m_captureChannel = new CFifoCapture();

        m_inputFramesThread = new BlueFishInputThread( this );

        m_inputFramesThread->Start();
    }

    if( output )
    {
        m_playbackData = std::move( output );
        m_playbackFifoBuffer = new CFifoBuffer();
        m_playbackChannel = new CFifoPlayback();

        m_frameProcessingThread = new BlueFishVCThread( this, 1920 * 1080 * 4 ); // FIXME: Set frame size properly.

        if( m_playbackData->interlaced )
        {
            m_frameProcessingThread->EnableInterlacing( true );
            //m_frameProcessingThread->SetFrameDuration( UInt64( 1000 / ( float(output->refresh) / 100.f ) ) );
        }

        m_frameProcessingThread->Start();
    }
}

//**************************************
//
Channel::~Channel()
{
    if( m_captureChannel )
    {
        if( m_captureData->playthrough )
        {
            m_PlaythroughThreadArgs.bDoRun = FALSE;
            WaitForSingleObject( m_PlaythroughThreadHandle, INFINITE );
            CloseHandle( m_PlaythroughThreadHandle );
        }

        m_captureChannel->StopThread();
        delete m_captureChannel;
        m_captureChannel = nullptr;

        m_inputFramesThread->Kill();
        m_inputFramesThread->EnqueueEndMessage();
        m_inputFramesThread->Join();

        delete m_inputFramesThread;
        m_inputFramesThread = nullptr;
    }

    if( m_playbackChannel )
    {
        delete m_playbackChannel;
        m_playbackChannel = nullptr;

        m_frameProcessingThread->Kill();
        m_frameProcessingThread->EnqueueEndMessage();
        m_frameProcessingThread->Join();

        delete m_frameProcessingThread;
        m_frameProcessingThread = nullptr;
    }
}

//**************************************
//
ChannelName Channel::GetName() const
{
    return m_channelName;
}

//**************************************
//
IOType      Channel::GetOutputType      () const
{
    if( m_playbackData )
    {
        return m_playbackData->type;
    }

    return IOType::INVALID;
}

//**************************************
//
IOType      Channel::GetInputType       () const
{
    if( m_captureData )
    {
        return m_captureData->type;
    }

    return IOType::INVALID;
}

//**************************************
//
UInt32      Channel::GetOutputChannel           () const
{
    if( m_playbackData )
    {
        return ChannelOptions.at( m_channelName ).OutputChannel;
    }

    return 0;
}

//**************************************
//
UInt64      Channel::GetOutputId                 () const
{
    if( m_playbackData )
    {
        return m_playbackData->linkedVideoOutput;
    }

    return MAXDWORD64;
}

// ***********************
//

VideoInputID Channel::GetInputId() const
{
    if( m_captureData )
    {
        return m_captureData->linkedVideoInput;
    }

    return MAXDWORD32;
}

//**************************************
//
UInt32      Channel::GetEpochSDIOutput          () const
{
    if( m_playbackData )
    {
        return ChannelOptions.at( m_channelName ).EpochSDIOutput;
    }

    return 0;
}

//**************************************
//
UInt32      Channel::GetEpochSDIKeyOutput       () const
{
	if( m_playbackData )
	{
		return ChannelOptions.at( m_channelName ).EpochSDIKeyOutput;
	}

	return 0;
}


//**************************************
//
UInt32      Channel::GetEpochOutputMemInterface () const
{
    if( m_playbackData )
    {
        return ChannelOptions.at( m_channelName ).EpochOutputMemInterface;
    }

    return 0;
}

//**************************************
//
UInt32      Channel::GetInputChannel            () const
{
    if( m_captureData )
    {
        return ChannelOptions.at( m_channelName ).InputChannel;
    }

    return 0;
}

//**************************************
//
UInt32      Channel::GetEpochSDIInput           () const
{
    if( m_captureData )
    {
        return ChannelOptions.at( m_channelName ).EpochSDIInput;
    }

    return 0;
}

//**************************************
//
UInt32      Channel::GetEpochInputMemInterface  () const
{
    if( m_captureData )
    {
        return ChannelOptions.at( m_channelName ).EpochInputMemInterface;
    }

    return 0;
}

// ***********************
//
UInt32          Channel::GetResolution          () const
{
    if( m_captureData )
    {
        return m_captureData->resolution;
    }
    else if( m_playbackData )
    {
        return m_playbackData->resolution;
    }

    return 0;
}

//**************************************
//
CFifoCapture *  Channel::GetCaptureChannel      () const
{
    return m_captureChannel;
}

//**************************************
//
CFifoPlayback * Channel::GetPlaybackChannel     () const
{
    return m_playbackChannel;
}

//**************************************
//
CFifoBuffer *   Channel::GetCaptureBuffer       ()
{
    return m_captureFifoBuffer;
}

//**************************************
//
CFifoBuffer *   Channel::GetPlaybackBuffer      ()
{
    return m_playbackFifoBuffer;
}

//**************************************
//
bool            Channel::PlaythroughEnabled     () const
{
    if( m_captureChannel )
    {
        return ( UInt32 )m_captureData->playthrough;
    }

    return false;
}

//**************************************
//
UInt32          Channel::GetVideoMode           () const
{
    if( m_playbackData )
    {
        return ( UInt32 )m_playbackData->videoMode;
    }

    return m_captureChannel->m_nVideoMode;
}

//**************************************
//
UInt32          Channel::GetReferenceMode       () const
{
    if( m_playbackData )
    {
        return ( UInt32 )m_playbackData->referenceMode;
    }

    return 0;
}

//**************************************
//
UInt32          Channel::GetReferenceH          () const
{
    if( m_playbackData )
    {
        return ( UInt32 )m_playbackData->referenceH;
    }

    return 0;
}

//**************************************
//
UInt32          Channel::GetReferenceV          () const
{
    if( m_playbackData )
    {
        return ( UInt32 )m_playbackData->referenceV;
    }

    return 0;
}

//**************************************
//
bool            Channel::GetFlipped             () const
{
    if( m_playbackData )
    {
        return m_playbackData->flipped;
    }

    return false;
}

//**************************************
//
UInt32          Channel::GetUpdateFormat        () const
{
    if( m_playbackData )
    {
        return m_playbackData->updateFormat;
    }
    else if( m_captureData )
    {
        return m_captureData->updateFormat;
    }

    return 0;
}

//**************************************
//
UInt32          Channel::GetMemoryFormat        () const
{
    if( m_playbackData )
    {
        return m_playbackData->memoryFormat;
    }
    else if( m_captureData )
    {
        return m_captureData->memoryFormat;
    }

    return false;
}

//**************************************
//
bool            Channel::AutoGeneratedTimecode  () const
{
    if( m_playbackData )
    {
        return m_playbackData->autoGeneratedTimecode;
    }

    return false;
}

//**************************************
//
bool            Channel::TimecodePresented      () const
{
    if( m_playbackData )
    {
        return m_playbackData->timecodePresented;
    }

    return false;
}


//**************************************
//
void Channel::StartThreads()
{
    if( m_captureData && m_captureData->playthrough )
    {
        m_PlaythroughThreadArgs = MainThreadArgs();
        m_PlaythroughThreadArgs.bDoRun = TRUE;
        m_PlaythroughThreadArgs.pInputFifo = m_captureFifoBuffer;
        m_PlaythroughThreadArgs.pOutputFifo = m_playbackFifoBuffer;
        m_PlaythroughThreadArgs.channel = this;
        m_PlaythroughThreadID = 0;
        m_PlaythroughThreadHandle = ( HANDLE )_beginthreadex( NULL, 0, &PlaythroughThread, &m_PlaythroughThreadArgs, CREATE_SUSPENDED, &m_PlaythroughThreadID );
        if( !m_PlaythroughThreadHandle )
        {
            //std::cout << "Error starting Main Thread StartDuplexThread" << std::endl;
            /*delete m_captureChannel;
            delete m_playbackChannel;*/
            return;
        }
        SetThreadPriority( m_PlaythroughThreadHandle, THREAD_PRIORITY_TIME_CRITICAL );
    }

    if( m_captureChannel )
    {
        m_captureChannel->StartThread();
    }

    if( m_playbackChannel )
    {
        m_playbackChannel->StartThread();
    }

    if( m_captureData && m_captureData->playthrough )
    {
        ResumeThread( m_PlaythroughThreadHandle );
    }
}

//**************************************
//
void Channel::StopThreads()
{
    if( m_playbackChannel )
    {
        m_playbackChannel->StopThread();
    }

    if( m_captureChannel )
    {
        m_captureChannel->StopThread();
    }

    //FIXME: playthrough
}

//**************************************
//
void Channel::SuspendThreads()
{
    if( m_playbackChannel )
    {
        m_playbackChannel->SuspendThread();
    }

    if( m_captureChannel )
    {
        m_captureChannel->SuspendThread();
    }
    //FIXME: playthrough
}

//**************************************
//
void Channel::ResumeThreads()
{
    if( m_captureChannel )
    {
        m_captureChannel->StartThread();
    }

    if( m_playbackChannel )
    {
        m_playbackChannel->StartThread();
    }
    //FIXME: playthrough
}

//**************************************
//
void Channel::EnqueueFrame          ( const AVFrameConstPtr & frame )
{
    m_frameProcessingThread->EnqueueFrame( frame );
}

// ***********************
//
AVFramePtr              Channel::QueryInputFrame()
{
    if( IsInputChannel() )
        return m_inputFramesThread->PopNextFrame();
    return nullptr;
}

//**************************************
//
void Channel::SetVideoOutput        ( bool enable )
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
bool        Channel::IsInputChannel     () const
{
    return m_captureChannel != nullptr;
}

// ***********************
//
bool        Channel::IsOutputChannel    () const
{
    return m_playbackChannel != nullptr;
}

// ***********************
//
AVFrameDescriptor       Channel::CreateFrameDesc        () const
{
    AVFrameDescriptor frameDesc;

    frameDesc.height = GetResolution();
    frameDesc.width = Resolution2Width( frameDesc.height );
    frameDesc.depth = 4;

    return frameDesc;
}

//**************************************
//
void        Channel::UpdateFrameTime      ( UInt64 t )
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_lastFrameTime = t;
}

//**************************************
//
UInt64      Channel::GetFrameTime         () const
{
    std::unique_lock< std::mutex > lock( m_mutex );
    return m_lastFrameTime;
}

//**************************************
//
void Channel::FrameProcessed	    ( const AVFrameConstPtr & frame )
{
    auto playbackChannel = GetPlaybackChannel();
    if( playbackChannel && !PlaythroughEnabled() )
    {
        BVTimeCode tc; // FIXME: https://www.pivotaltracker.com/story/show/145508031
        tc.h = 10;
        tc.m = 22;
        tc.s = 33;
        tc.frame = 12;
        playbackChannel->m_pFifoBuffer->PushFrame(
            std::make_shared< CFrame >( reinterpret_cast< const unsigned char * >( frame->m_videoData->Get() ),
                                        playbackChannel->GoldenSize,
                                        playbackChannel->BytesPerLine,
                                        !m_odd,
                                        frame->m_audioData ? ( unsigned int ) frame->m_audioData->Size() : 0,
                                        frame->m_audioData ? reinterpret_cast< const unsigned char * >( frame->m_audioData->Get() ) : nullptr,
                                        tc,
                                        frame->m_desc
                                        ) );

        m_odd = ( m_odd + 1 ) % 2;
    }
}

//**************************************
//
unsigned int __stdcall Channel::PlaythroughThread(void * pArg)
{
    MainThreadArgs * pParams = ( MainThreadArgs * )pArg;

    while( pParams->bDoRun )
    {   
        pParams->channel->UpdateFrameTime( Time::Now() );
        pParams->pOutputFifo->PushFrame( pParams->pInputFifo->PopFrame() );
    }

    _endthreadex(0);
    return 0;
}


//**************************************
//
Channel::ChannelOptionMap     Channel::CreateChannelOptionMap   ()
{
    ChannelOptionMap channelOptionMap;

    ChannelOption A;
    A.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_A;
    A.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_A;
    A.EpochSDIInput = EPOCH_SRC_SDI_INPUT_A;
    A.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_A;
	A.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_B;
	A.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHA;
    A.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA;
	

    ChannelOption B;
    B.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_B;
    B.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_B;
    B.EpochSDIInput = EPOCH_SRC_SDI_INPUT_B;
    B.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_B;
	B.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_C;
    B.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHB;
    B.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHB;

    ChannelOption C;
    C.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_C;
    C.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_C;
    C.EpochSDIInput = EPOCH_SRC_SDI_INPUT_C;
    C.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_C;
	C.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_D;
    C.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHC;
    C.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHC;

    ChannelOption D;
    D.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_D;
    D.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_D;
    D.EpochSDIInput = EPOCH_SRC_SDI_INPUT_D;
    D.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_D;
	D.EpochSDIKeyOutput = EPOCH_DEST_SDI_OUTPUT_D;
    D.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHD;
    D.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHD;

    channelOptionMap[ ChannelName::A ] = A;
    channelOptionMap[ ChannelName::B ] = B;
    channelOptionMap[ ChannelName::C ] = C;
    channelOptionMap[ ChannelName::D ] = D;

    return channelOptionMap;
}

} //bluefish
} //videocards
} //bv
