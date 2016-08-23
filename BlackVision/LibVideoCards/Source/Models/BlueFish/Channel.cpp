#include "Channel.h"

#include <map>


namespace bv { namespace videocards { namespace bluefish {


//**************************************
//
Channel::Channel( ChannelName name, InputDataUPtr & input, OutputDataUPtr & output )
    : m_channelName( name )
    , m_captureData( nullptr )
    , m_captureChannel( nullptr )
    , m_captureFifoBuffer( nullptr )
    , m_playbackData( nullptr )
    , m_playbackChannel( nullptr )
    , m_playbackFifoBuffer( nullptr )
{
    if( input ) 
    {
        m_captureData = std::move( input );
        m_captureFifoBuffer = new CFifoBuffer();
        m_captureChannel = new CFifoCapture();
    }

    if( output )
    {
        m_playbackData = std::move( output );
        m_playbackFifoBuffer = new CFifoBuffer();
        m_playbackChannel = new CFifoPlayback();
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
    }

    if( m_playbackChannel )
    {
        m_playbackChannel->StopThread();
        delete m_playbackChannel;
        m_playbackChannel = nullptr;
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
IOType      Channel::GetOutputType() const
{
    if( m_playbackData )
    {
        return m_playbackData->type;
    }

    return IOType::INVALID;
}

//**************************************
//
IOType      Channel::GetInputType() const
{
    if( m_captureData )
    {
        return m_captureData->type;
    }

    return IOType::INVALID;
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

////**************************************
////
//bool          Channel::HasPlaythroughChannel    () const
//{
//    if( m_captureChannel )
//    {
//        m_captureChannel->
//    }
//    return false;
//}

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
        m_PlaythroughThreadID = 0;
        m_PlaythroughThreadHandle = ( HANDLE )_beginthreadex( NULL, 0, &PlaythroughThread, &m_PlaythroughThreadArgs, CREATE_SUSPENDED, &m_PlaythroughThreadID );
        if( !m_PlaythroughThreadHandle )
        {
            std::cout << "Error starting Main Thread StartDuplexThread" << std::endl;
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
void Channel::EnableVideoOutput     ()
{    
    if( GetPlaybackChannel() && GetPlaybackChannel()->m_pSDK )
    {
        VARIANT value;       
		value.vt = VT_UI4;

        value.ulVal = ENUM_BLACKGENERATOR_OFF;
        GetPlaybackChannel()->m_pSDK->SetCardProperty( VIDEO_BLACKGENERATOR, value );
    }
}

//**************************************
//
void Channel::DisableVideoOutput     ()
{    
    if( GetPlaybackChannel() && GetPlaybackChannel()->m_pSDK )
    {
        VARIANT value;       
		value.vt = VT_UI4;

        value.ulVal = ENUM_BLACKGENERATOR_ON;
        GetPlaybackChannel()->m_pSDK->SetCardProperty( VIDEO_BLACKGENERATOR, value );
    }
}

//**************************************
//
unsigned int __stdcall Channel::PlaythroughThread(void * pArg)
{
    MainThreadArgs * pParams = ( MainThreadArgs * )pArg;

    while( pParams->bDoRun )
    {   
        pParams->pOutputFifo->PutLiveBuffer( pParams->pInputFifo->GetLiveBuffer() );
    }

    _endthreadex(0);
    return 0;
}

} //bluefish
} //videocards
} //bv
