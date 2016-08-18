#include "Channel.h"


namespace bv { namespace videocards { namespace bluefish {

//**************************************
//
std::pair< ChannelName, const char * > ChannelNameMapping[] = 
{   std::make_pair( ChannelName::A, "A" )
    , std::make_pair( ChannelName::B, "B" )
    , std::make_pair( ChannelName::C, "C" )
    , std::make_pair( ChannelName::D, "D" )
};

//**************************************
//
std::pair< IOType, const char * > IOTypeMapping[] = 
{   std::make_pair( IOType::FILL_KEY, "FILL_KEY" )
    , std::make_pair( IOType::FILL, "FILL" )
    , std::make_pair( IOType::KEY, "KEY" )
};


//**************************************
//
template<> ChannelName          String2T        ( const std::string & s )   {  return String2Enum( ChannelNameMapping, s ); }

//**************************************
//
template<> std::string          T2String        ( const ChannelName & t )   { return Enum2String( ChannelNameMapping, t ); }

//**************************************
//
template<> IOType               String2T        ( const std::string & s )   { return String2Enum( IOTypeMapping, s ); }

//**************************************
//
template<> std::string          T2String        ( const IOType & t )        { return Enum2String( IOTypeMapping, t ); }



//**************************************
//
Channel::Channel( std::string name, std::string outputType, unsigned short resolution, unsigned short refresh, bool interlaced, bool playback, bool capture,bool playthrough, std::string inputType, long referenceMode, int refH, int refV )
{
    m_channelName = String2T< ChannelName >( name );
    m_inputType = String2T< IOType >( inputType );
    m_outputType = String2T< IOType >( outputType );

    m_playback = playback;
    m_capture = capture;
    m_playthrough = playthrough;

    m_videoMode = ParseVideoMode( resolution, refresh, interlaced );

    m_referenceMode = referenceMode;
    m_refH = refH;
    m_refV = refV;
    if(capture) this->m_CaptureChannel = new CFifoCapture();
    if(playback) this->m_PlaybackChannel = new CFifoPlayback();
}

//**************************************
//
Channel::~Channel()
{
    std::cout << "Deleting Channels.... " << std::endl;
    if(m_playthrough)
    {
        m_PlaythroughThreadArgs.bDoRun = FALSE;
        WaitForSingleObject(m_PlaythroughThreadHandle, INFINITE);
        CloseHandle(m_PlaythroughThreadHandle);
    }

    if(m_Playback)
    {
        m_PlaybackChannel->StopPlaybackThread();
        m_PlaybackChannel->m_pFifoBuffer->m_threadsafebuffer.clear();
        delete m_PlaybackChannel;
        m_PlaybackChannel = NULL;
    }

    if(m_Capture)
    {
        m_CaptureChannel->m_pFifoBuffer->m_threadsafebuffer.clear();

        m_CaptureChannel->StopCaptureThread();
        delete m_CaptureChannel;
        m_CaptureChannel = NULL;
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
    return m_outputType;
}

//**************************************
//
IOType      Channel::GetInputType() const
{
    return m_inputType;
}

//**************************************
//
CFifoBuffer* Channel::GetCaptureBuffer()
{
    return &m_CaptureFifoBuffer;
}

//**************************************
//
CFifoBuffer* Channel::GetPlaybackBuffer()
{
    return &m_PlaybackFifoBuffer;
}

//**************************************
//
void Channel::InitThreads()
{
    if(m_Capture)this->m_CaptureChannel->InitThread();
    if(m_Playback) this->m_PlaybackChannel->InitThread();
}

//**************************************
//
void Channel::StartThreads()
{
    if(m_Capture)this->m_CaptureChannel->StartCaptureThread();
    if(m_Playback)this->m_PlaybackChannel->StartPlaybackThread();
}

//**************************************
//
void Channel::StopThreads()
{
    if(m_Playback)this->m_PlaybackChannel->StopPlaybackThread();
    if(m_Capture)this->m_CaptureChannel->StopCaptureThread();
}

//**************************************
//
void Channel::SuspendThreads()
{
    if(m_Playback)this->m_PlaybackChannel->SuspendPlaybackThread();
    if(m_Capture)this->m_CaptureChannel->SuspendCaptureThread();
}

//**************************************
//
void Channel::ResumeThreads()
{
    if(m_Capture)this->m_CaptureChannel->ResumeCaptureThread();
    if(m_Playback)this->m_PlaybackChannel->ResumePlaybackThread();
}

//**************************************
//
CFifoCapture* Channel::GetCaptureChannel()
{
    return m_CaptureChannel;
}

//**************************************
//
CFifoPlayback* Channel::GetPlaybackChannel()
{
    return m_PlaybackChannel;
}

//**************************************
//
void Channel::GenerateBlack()
{    
    if(GetPlaybackChannel()->m_pSDK)
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
    std::cout << "Thread " << pParams->strChannel.c_str() << " running." << std::endl;

    while(pParams->bDoRun)
    {   
        pParams->pOutputFifo->m_threadsafebuffer.push(pParams->pInputFifo->m_threadsafebuffer.pop());
    }

    std::cout << "PlaythroughThread " << pParams->strChannel.c_str() << " exiting." << std::endl;
    _endthreadex(0);
    return 0;
}

//**************************************
//
void Channel::StartDuplexThread()
{
    if(m_playthrough)
    {
        m_PlaythroughThreadArgs = MainThreadArgs();
        m_PlaythroughThreadArgs.bDoRun = TRUE;
        m_PlaythroughThreadArgs.pInputFifo = GetCaptureBuffer();//>m_CaptureFifoBuffer;
        m_PlaythroughThreadArgs.pOutputFifo = GetPlaybackBuffer();//&this->m_PlaybackFifoBuffer;
        m_PlaythroughThreadArgs.strChannel = this->m_Name;
        m_PlaythroughThreadID = 0;
        m_PlaythroughThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &PlaythroughThread, &m_PlaythroughThreadArgs, CREATE_SUSPENDED, &m_PlaythroughThreadID);
        if(!m_PlaythroughThreadHandle)
        {
            std::cout << "Error starting Main Thread StartDuplexThread" << std::endl;
            delete this->m_CaptureChannel;
            delete this->m_PlaybackChannel;
        }
        SetThreadPriority(m_PlaythroughThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
    }

    if(m_Capture) this->m_CaptureChannel->StartCaptureThread(); //this actually just resumes the threads; it would take too long to start them from scracth
    if(m_Playback) this->m_PlaybackChannel->StartPlaybackThread();  //that's why we created them before as suspended

    if(m_playthrough) ResumeThread(m_PlaythroughThreadHandle);
}

//**************************************
//
ULONG Channel::ParseVideoMode(unsigned short resolution, unsigned short refresh, bool interlaced)
{
    ULONG VideoMode=VID_FMT_INVALID;
    if(interlaced)
    {
        switch(resolution)
        {
         case 1080:
             {
                switch(refresh)
                {
                 case 5000:
                     {
                         VideoMode = VID_FMT_1080I_5000;
                         break;
                     }
                 case 5994:
                     {
                         VideoMode = VID_FMT_1080I_5994;
                         break;
                     }
                 case 6000:
                     {
                         VideoMode = VID_FMT_1080I_6000;
                         break;
                     }
                 default:
                     {
                         std::cout << "Unknown refresh rate for Interlaced 1080 resolution" << std::endl;
                     }
                }
                break;
             }
         case 576:
             {
                switch(refresh)
                {
                 case 5000:
                     {
                         VideoMode = VID_FMT_576I_5000;
                         break;
                     }
                 default:
                     {
                         std::cout << "Unknown refresh rate for Interlaced 576 resolution" << std::endl;
                     }
                }
                break;
             }
         case 486:
             {
                switch(refresh)
                {
                 case 5994:
                     {
                         VideoMode = VID_FMT_486I_5994;
                         break;
                     }
                 default:
                     {
                         std::cout << "Unknown refresh rate for Interlaced 486 resolution" << std::endl;
                     }
                }
                break;
             }
         default:
             {
                 std::cout << "Unknown Resolution for Interlaced Setting" << std::endl;
                 break;
             }
        }
    }
    else
    {
        switch(resolution)
        {
         case 1080:
             {
                switch(refresh)
                {
                 case 2397:
                     {
                         VideoMode = VID_FMT_1080P_2397;
                         break;
                     }
                 case 2400:
                     {
                         VideoMode = VID_FMT_1080P_2400;
                         break;
                     }
                 case 2500:
                     {
                         VideoMode = VID_FMT_1080P_2500;
                         break;
                     }
                 case 2997:
                     {
                         VideoMode = VID_FMT_1080P_2997;
                         break;
                     }
                 case 3000:
                     {
                         VideoMode = VID_FMT_1080P_3000;
                         break;
                     }
                 case 4800:
                     {
                         VideoMode = VID_FMT_1080P_4800;
                         break;
                     }
                 case 5000:
                     {
                         VideoMode = VID_FMT_1080P_5000;
                         break;
                     }
                 case 5994:
                     {
                         VideoMode = VID_FMT_1080P_5994;
                         break;
                     }
                 case 6000:
                     {
                         VideoMode = VID_FMT_1080P_6000;
                         break;
                     }
                 default:
                     {
                         std::cout << "Unknown refresh rate for Progressive 1080 resolution" << std::endl;
                     }
                }
                break;
             }
         case 720:
             {
                switch(refresh)
                {
                 case 2398:
                     {
                         VideoMode = VID_FMT_720P_2398;
                         break;
                     }
                 case 2400:
                     {
                         VideoMode = VID_FMT_720P_2400;
                         break;
                     }
                 case 2500:
                     {
                         VideoMode = VID_FMT_720P_2500;
                         break;
                     }
                 case 2997:
                     {
                         VideoMode = VID_FMT_720P_2997;
                         break;
                     }
                 case 3000:
                     {
                         VideoMode = VID_FMT_720P_3000;
                         break;
                     }
                 case 5000:
                     {
                         VideoMode = VID_FMT_720P_5000;
                         break;
                     }
                 case 5994:
                     {
                         VideoMode = VID_FMT_720P_5994;
                         break;
                     }
                 case 6000:
                     {
                         VideoMode = VID_FMT_720P_6000;
                         break;
                     }
                 default:
                     {
                         std::cout << "Unknown refresh rate for Progressive 720 resolution" << std::endl;
                     }
                }
                break;
             }
         default:
             {
                std::cout << "Unknown resolution" << std::endl;
                break;
             }
        }
    }

    if(VideoMode==VID_FMT_INVALID)  
        std::cout << "UNKNOWN RESOLUTION/REFRESH RATE CONFIGURATION" << std::endl;

    return VideoMode;
}

} //bluefish
} //videocards
} //bv
