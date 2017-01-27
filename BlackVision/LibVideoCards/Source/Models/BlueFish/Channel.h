#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"

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

    typedef std::map< ChannelName, ChannelOption > ChannelOptionMap;

private:

    MainThreadArgs  m_PlaythroughThreadArgs;

    CFifoBuffer *   m_captureFifoBuffer;
    CFifoCapture *  m_captureChannel;

    CFifoBuffer *   m_playbackFifoBuffer;
    CFifoPlayback * m_playbackChannel;

    ChannelName     m_channelName;

    unsigned int    m_PlaythroughThreadID;
    
public:

    ChannelInputDataUPtr    m_captureData;
    ChannelOutputDataUPtr   m_playbackData;

    HANDLE                  m_PlaythroughThreadHandle;

	static ChannelOptionMap ChannelOptions;

public:

                    Channel             ( ChannelName name, ChannelInputDataUPtr & input, ChannelOutputDataUPtr & output );
                    ~Channel            ();

    ChannelName     GetName             () const;

    IOType          GetOutputType       () const;
    IOType          GetInputType        () const;

    UInt32          GetOutputChannel            () const;
    UInt32          GetEpochSDIOutput           () const;
    UInt32          GetEpochOutputMemInterface  () const;
    UInt32          GetInputChannel             () const;
    UInt32          GetEpochSDIInput            () const;
	UInt32			GetEpochSDIKeyOutput		() const;
    UInt32          GetEpochInputMemInterface   () const;

    CFifoCapture *  GetCaptureChannel   () const;
    CFifoPlayback * GetPlaybackChannel  () const;

    CFifoBuffer *   GetCaptureBuffer    ();
    CFifoBuffer *   GetPlaybackBuffer   ();
    
    bool            PlaythroughEnabled  () const;

    UInt32          GetVideoMode        () const;
    UInt32          GetReferenceMode    () const;
    UInt32          GetReferenceH       () const;
    UInt32          GetReferenceV       () const;
    bool            GetFlipped          () const;
    UInt32          GetUpdateFormat     () const;
    UInt32          GetMemoryFormat     () const;

    void            StartThreads        ();
    void            StopThreads         ();
    void            SuspendThreads      ();
    void            ResumeThreads       ();

    void            SetVideoOutput      ( bool enable );

    unsigned int static __stdcall PlaythroughThread ( void * pArg );

    static ChannelOptionMap CreateChannelOptionMap      ();

};

} //bluefish
} //videocards
} //bv