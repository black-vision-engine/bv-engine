#pragma once
#include "win_sock.h"
#include <string>

#include "BlueFish/inc/BlueVelvet4.h"
#include "BlueFish/inc/BlueGpuDirect.h"
#include "BlueFish/inc/BlueHancUtils.h"

#include "Utils.h"
#include "FifoBuffer.h"
#include "FifoCapture.h"
#include "FifoPlayback.h"


namespace bv { namespace videocards { namespace bluefish {


struct MainThreadArgs
{
    CFifoBuffer*    pInputFifo;
    CFifoBuffer*    pOutputFifo;
    std::string     strChannel;
    BOOL            bDoRun;
};

class Channel
{
private:

    MainThreadArgs  m_PlaythroughThreadArgs;
    CFifoBuffer     m_CaptureFifoBuffer;
    CFifoBuffer     m_PlaybackFifoBuffer;
    CFifoCapture*   m_CaptureChannel;
    CFifoPlayback*  m_PlaybackChannel;
    ChannelName     m_channelName;
    IOType          m_inputType;
    IOType          m_outputType;
    unsigned int    m_PlaythroughThreadID;
    
public:

    bool            m_playback;
    bool            m_capture;
    bool            m_playthrough;

    ULONG           m_videoMode;
    HANDLE          m_PlaythroughThreadHandle;

    std::shared_ptr<CFrame>         pFrameOut;
    long            m_referenceMode;
    int             m_refH;
    int             m_refV;

public:

    Channel( std::string name, std::string type, unsigned short resolution, unsigned short refresh, bool interlaced, bool flipped, bool playback, bool capture, bool playthrough, std::string inputType, long referenceMode, int refH, int refV);
    ~Channel();

    ChannelName     GetName             () const;
    IOType          GetOutputType       () const;
    IOType          GetInputType        () const;

    CFifoBuffer*    GetCaptureBuffer    ();
    CFifoBuffer*    GetPlaybackBuffer   ();
    CFifoCapture*   GetCaptureChannel   ();
    CFifoPlayback*  GetPlaybackChannel  ();

    ULONG           ParseVideoMode      ( unsigned short resolution, unsigned short refresh, bool interlaced );
    void            InitThreads         ();
    void            StartThreads        ();
    void            StopThreads         ();
    void            SuspendThreads      ();
    void            ResumeThreads       ();
    void            StartDuplexThread   ();
    void            GenerateBlack       ();
    unsigned int static __stdcall PlaythroughThread ( void * pArg );

};

} //bluefish
} //videocards
} //bv