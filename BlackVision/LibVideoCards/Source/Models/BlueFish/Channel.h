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

#include "CoreDEF.h"


namespace bv { namespace videocards { namespace bluefish {


struct MainThreadArgs
{
    CFifoBuffer *   pInputFifo;
    CFifoBuffer *   pOutputFifo;
    BOOL            bDoRun;
};

class Channel
{
private:

    MainThreadArgs  m_PlaythroughThreadArgs;

    CFifoBuffer *   m_captureFifoBuffer;
    CFifoCapture *  m_captureChannel;

    CFifoBuffer *   m_playbackFifoBuffer;
    CFifoPlayback * m_playbackChannel;

    ChannelName     m_channelName;

    unsigned int    m_PlaythroughThreadID;
    
public:

    struct InputData
    {
        IOType                  type;
        bool                    playthrough;
    };

    DEFINE_UPTR_TYPE( InputData )

    struct OutputData
    {
        IOType                  type;
        UInt32                  resolution;
	    UInt32                  refresh;
        bool                    interlaced;
        bool                    flipped;
        _EVideoMode             videoMode;
        _EBlueGenlockSource     referenceMode;
        Int32                   referenceH;
        Int32                   referenceV;
    };

    DEFINE_UPTR_TYPE( OutputData )

public:

    InputDataUPtr   m_captureData;
    OutputDataUPtr  m_playbackData;

    HANDLE          m_PlaythroughThreadHandle;

    //std::shared_ptr<CFrame>         pFrameOut;
    //long            m_referenceMode;
    //int             m_refH;
    //int             m_refV;

public:

                    Channel             ( ChannelName name, InputDataUPtr & input, OutputDataUPtr & output );
                    ~Channel            ();

    ChannelName     GetName             () const;

    IOType          GetOutputType       () const;
    IOType          GetInputType        () const;

    CFifoCapture *  GetCaptureChannel   () const;
    CFifoPlayback * GetPlaybackChannel  () const;

    CFifoBuffer *   GetCaptureBuffer    ();
    CFifoBuffer *   GetPlaybackBuffer   ();
    
    UInt32          GetVideoMode        () const;
    UInt32          GetReferenceMode    () const;
    UInt32          GetReferenceH       () const;
    UInt32          GetReferenceV       () const;
    bool            GetFlipped          () const;

    //bool            HasPlaythroughChannel () const;

    void            StartThreads        ();
    void            StopThreads         ();
    void            SuspendThreads      ();
    void            ResumeThreads       ();

    void            EnableVideoOutput   ();
    void            DisableVideoOutput  ();

    unsigned int static __stdcall PlaythroughThread ( void * pArg );

};

} //bluefish
} //videocards
} //bv