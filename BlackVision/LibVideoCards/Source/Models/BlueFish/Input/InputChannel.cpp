#include "InputChannel.h"


#include "UseLoggerVideoModule.h"


namespace bv {
namespace videocards {
namespace bluefish
{



// ***********************
//
InputChannel::InputChannel( ChannelName name, ChannelInputDataUPtr & input )
    :   Channel( name )
    ,   CaptureData( std::move( input ) )
    ,   m_captureChannel( nullptr )
    ,   m_captureFifoBuffer( nullptr )
    ,   m_inputFramesThread( nullptr )
    ,   m_stopped( false )
{
    m_captureFifoBuffer = new CFifoBuffer();
    m_captureChannel = new CFifoCapture();

    m_inputFramesThread = new BlueFishInputThread( this );
    m_inputFramesThread->Start();
}

// ***********************
//
InputChannel::~InputChannel()
{
    m_captureChannel->StopThread();

    m_inputFramesThread->Kill();
    m_inputFramesThread->EnqueueEndMessage();
    m_inputFramesThread->Join();

    delete m_inputFramesThread;
    m_inputFramesThread = nullptr;

    // Note: m_inputFramesThread uses m_captureChannel
    delete m_captureChannel;
    m_captureChannel = nullptr;
}

// ***********************
//
AVFramePtr          InputChannel::QueryInputFrame       ()
{
    return m_inputFramesThread->PopNextFrame();
}

// ***********************
//
CFifoCapture *      InputChannel::GetCaptureChannel     () const
{
    return m_captureChannel;
}

// ***********************
//
CFifoBuffer *       InputChannel::GetCaptureBuffer      ()
{
    return m_captureFifoBuffer;
}

// ***********************
//
void                InputChannel::SetVideoInput         ( bool enable )
{
    if( enable && m_stopped )
    {
        ResumeInput();
        m_stopped = false;
    }
    else if( !enable && !m_stopped )
    {
        StopInput();
        m_stopped = true;
    }
    else if( m_stopped )
    {
        assert( false );
        LOG_MESSAGE( SeverityLevel::warning ) << "InputChannel already stopped. Unexpected call to SetVideoInput( false ).";
    }
    else if( !m_stopped )
    {
        assert( false );
        LOG_MESSAGE( SeverityLevel::warning ) << "InputChannel is already running. Unexpected call to SetVideoInput( true ).";
    }
}

// ***********************
//
IOType              InputChannel::GetInputType          () const
{
    return CaptureData->type;;
}

// ***********************
//
VideoInputID        InputChannel::GetInputId            () const
{
    return CaptureData->linkedVideoInput;
}

// ***********************
//
UInt32              InputChannel::GetInputChannel       () const
{
    return ChannelOptions.at( m_channelName ).InputChannel;
}

// ***********************
//
UInt32              InputChannel::GetEpochSDIInput      () const
{
    return ChannelOptions.at( m_channelName ).EpochSDIInput;
}

// ***********************
//
UInt32              InputChannel::GetEpochInputMemInterface () const
{
    return ChannelOptions.at( m_channelName ).EpochInputMemInterface;
}

// ***********************
//
UInt32              InputChannel::GetVideoMode          () const
{
    return m_captureChannel->m_nVideoMode;
}

// ***********************
//
UInt32              InputChannel::GetUpdateFormat       () const
{
    return CaptureData->updateFormat;
}

// ***********************
//
UInt32              InputChannel::GetMemoryFormat       () const
{
    return CaptureData->memoryFormat;
}

// ***********************
//
UInt32          InputChannel::GetResolution             () const
{
    return CaptureData->resolution;
}

// ***********************
//
void            InputChannel::StartThreads          ()
{
    m_captureChannel->StartThread();
}

// ***********************
//
void            InputChannel::StopThreads           ()
{
    m_captureChannel->StopThread();
}

// ***********************
//
void            InputChannel::SuspendThreads        ()
{
    m_captureChannel->SuspendThread();
}

// ***********************
//
void            InputChannel::ResumeThreads         ()
{
    m_captureChannel->StartThread();
}

// ***********************
//
bool            InputChannel::IsInputChannel        () const
{
    return true;
}

// ***********************
//
bool            InputChannel::IsOutputChannel       () const
{
    return false;
}

// ***********************
//
void            InputChannel::StopInput             ()
{
    m_inputFramesThread->Stop();
}

// ***********************
//
void            InputChannel::ResumeInput           ()
{
    m_inputFramesThread->IgnoreFirstFrames();
    m_inputFramesThread->Resume();
}


} //bluefish
} //videocards
} //bv

