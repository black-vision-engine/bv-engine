#include "Channel.h"

#include <map>


namespace bv { namespace videocards { namespace bluefish {


//**************************************
//
Channel::Channel( ChannelName name, InputDataUPtr & input, OutputDataUPtr & output )
    : m_channelName( name )
    , m_captureData( nullptr )
    , m_captureChannel( nullptr )
    , m_playbackData( nullptr )
    , m_playbackChannel( nullptr )
{
    if( input ) 
    {
        m_captureData = std::move( input );
        m_captureChannel = new CFifoCapture();
    }

    if( output )
    {
        m_playbackData = std::move( output );
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
            WaitForSingleObject(m_PlaythroughThreadHandle, INFINITE);
            CloseHandle(m_PlaythroughThreadHandle);
        }

        m_captureChannel->StopCaptureThread();
        m_captureChannel->m_pFifoBuffer->m_threadsafebuffer.clear();
        delete m_captureChannel;
        m_captureChannel = nullptr;
    }

    if( m_playbackChannel )
    {
        m_playbackChannel->StopPlaybackThread();
        m_playbackChannel->m_pFifoBuffer->m_threadsafebuffer.clear();
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
CFifoBuffer* Channel::GetCaptureBuffer          () const
{
    return &m_captureFifoBuffer;
}

//**************************************
//
CFifoBuffer* Channel::GetPlaybackBuffer         () const
{
    return &m_playbackFifoBuffer;
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
bool          Channel::HasPlaythroughChannel    () const
{
    if( m_captureChannel )
    {
        m_captureChannel->
    }
    return false;
}


//**************************************
//
void Channel::InitThreads()
{
    if( m_captureChannel )
    {
        m_captureChannel->InitThread();
    }

    if( m_playbackChannel )
    {
        m_playbackChannel->InitThread();
    }
}

//**************************************
//
void Channel::StartThreads()
{
    if( m_captureChannel )
    {
        m_captureChannel->StartThread();
    }

    if( m_playbackChannel )
    {
        m_playbackChannel->StartThread();
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
}

//**************************************
//
void Channel::GenerateBlack()
{    
    if( GetPlaybackChannel()->m_pSDK )
    {
        VARIANT varVal;       
        varVal.ulVal = ENUM_BLACKGENERATOR_ON;
        GetPlaybackChannel()->m_pSDK->SetCardProperty(VIDEO_BLACKGENERATOR, varVal);
    }
    //
    //else
    //    cout << "BlueFish Playback SDK not INITIALISED" << endl;

   /* if(m_PlaybackChannel!=nullptr)
    {
        GetPlaybackBuffer()->PushKillerFrame();       
    }*/
}

//**************************************
//
unsigned int __stdcall Channel::PlaythroughThread(void * pArg)
{
    MainThreadArgs* pParams = (MainThreadArgs*)pArg;

    while(pParams->bDoRun)
    {   
        pParams->pOutputFifo->m_threadsafebuffer.push(pParams->pInputFifo->m_threadsafebuffer.pop());
    }

    _endthreadex(0);
    return 0;
}

//**************************************
//
void Channel::StartDuplexThread()
{
    if( m_captureData && m_captureData->playthrough )
    {
        m_PlaythroughThreadArgs = MainThreadArgs();
        m_PlaythroughThreadArgs.bDoRun = TRUE;
        m_PlaythroughThreadArgs.pInputFifo = GetCaptureBuffer();//>m_CaptureFifoBuffer;
        m_PlaythroughThreadArgs.pOutputFifo = GetPlaybackBuffer();//&this->m_PlaybackFifoBuffer;
        m_PlaythroughThreadID = 0;
        m_PlaythroughThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &PlaythroughThread, &m_PlaythroughThreadArgs, CREATE_SUSPENDED, &m_PlaythroughThreadID);
        if(!m_PlaythroughThreadHandle)
        {
            std::cout << "Error starting Main Thread StartDuplexThread" << std::endl;
            delete m_captureChannel;
            delete m_playbackChannel;
        }
        SetThreadPriority(m_PlaythroughThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
    }

    if( m_captureChannel )
    {
        m_captureChannel->StartThread(); //this actually just resumes the threads; it would take too long to start them from scracth
    }

    if( m_playbackChannel )
    {
        m_playbackChannel->StartThread();  //that's why we created them before as suspended
    }

    if( m_captureData && m_captureData->playthrough )
    {
        ResumeThread( m_PlaythroughThreadHandle );
    }
}

} //bluefish
} //videocards
} //bv
