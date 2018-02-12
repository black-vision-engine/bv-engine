#pragma once

#include "Models/BlueFish/Channel.h"

#include "Models/BlueFish/FifoBuffer.h"
#include "FifoCapture.h"

#include "BlueFishInputThread.h"



namespace bv {
namespace videocards {
namespace bluefish
{


// ***********************
//
class InputChannel : public Channel
{
private:

    CFifoBuffer *       m_captureFifoBuffer;
    CFifoCapture *      m_captureChannel;

    BlueFishInputThread *       m_inputFramesThread;

public:

    ChannelInputDataUPtr        CaptureData;

private:


public:

                        InputChannel        ( ChannelName name, ChannelInputDataUPtr & input );
    virtual             ~InputChannel       ();


    AVFramePtr          QueryInputFrame     ();

    CFifoCapture *      GetCaptureChannel   () const;
    CFifoBuffer *       GetCaptureBuffer    ();

    void                SetVideoInput       ( bool enable );

    IOType              GetInputType                () const;

    VideoInputID        GetInputId                  () const;
    UInt32              GetInputChannel             () const;
    UInt32              GetEpochSDIInput            () const;
    UInt32              GetEpochInputMemInterface   () const;

    UInt32              GetVideoMode                () const;
    UInt32              GetUpdateFormat             () const;
    UInt32              GetMemoryFormat             () const;

    virtual UInt32      GetResolution               () const override;

    virtual void        StartThreads                () override;
    virtual void        StopThreads                 () override;
    virtual void        SuspendThreads              () override;
    virtual void        ResumeThreads               () override;

    virtual bool        IsInputChannel              () const override;
    virtual bool        IsOutputChannel             () const override;
};





} //bluefish
} //videocards
} //bv
