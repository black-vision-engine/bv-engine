#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"

#include "FifoBuffer.h"
#include "FifoCapture.h"
#include "FifoPlayback.h"

#include "VideoInput/VideoInputChannelDesc.h"

#include "BlueFishInputThread.h"

#include "CoreDEF.h"

#include <atomic>

namespace bv { namespace videocards { namespace bluefish {

class BlueFishVCThread;
class Channel;

struct MainThreadArgs
{
    CFifoBuffer *   pInputFifo;
    CFifoBuffer *   pOutputFifo;
    Channel *       channel;
    BOOL            bDoRun;
};


// ***********************
/// @ingroup BlueFishVideoCard
class Channel
{
private:

    typedef std::map< ChannelName, ChannelOption > ChannelOptionMap;

protected:

    ChannelName     m_channelName;

public:

	static ChannelOptionMap ChannelOptions;

public:

                    Channel             ( ChannelName name );
    virtual         ~Channel            ();


    virtual void    StartThreads        () = 0;
    virtual void    StopThreads         () = 0;
    virtual void    SuspendThreads      () = 0;
    virtual void    ResumeThreads       () = 0;

    virtual bool    IsInputChannel      () const = 0;
    virtual bool    IsOutputChannel     () const = 0;

    virtual UInt32  GetResolution       () const = 0;
    ChannelName     GetName             () const;

    AVFrameDescriptor               CreateFrameDesc () const;


    static ChannelOptionMap CreateChannelOptionMap      ();

};

} //bluefish
} //videocards
} //bv