#include "InputChannel.h"





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



} //bluefish
} //videocards
} //bv

