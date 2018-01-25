#pragma once

#include "Models/BlueFish/Channel.h"

#include "Models/BlueFish/FifoBuffer.h"
#include "FifoPlayback.h"



namespace bv {
namespace videocards {
namespace bluefish
{

class BlueFishVCThread;


// ***********************
//
class OutputChannel : public Channel
{
private:

    CFifoBuffer *           m_playbackFifoBuffer;
    CFifoPlayback *         m_playbackChannel;

    BlueFishVCThread *      m_frameProcessingThread;

    std::atomic< int >      m_odd;
    mutable UInt64		    m_lastFrameTime;
    mutable std::mutex      m_mutex;

public:

    ChannelOutputDataUPtr   PlaybackData;

private:


public:

                        OutputChannel       ( ChannelName name, ChannelOutputDataUPtr & output );
    virtual             ~OutputChannel      ();


    CFifoBuffer *       GetPlaybackBuffer   ();
    CFifoPlayback *     GetPlaybackChannel  () const;


    IOType              GetOutputType               () const;

    UInt64              GetOutputId                 () const;
    UInt32              GetOutputChannel            () const;
    UInt32              GetEpochSDIOutput           () const;
    UInt32              GetEpochOutputMemInterface  () const;
    UInt32			    GetEpochSDIKeyOutput		() const;

    UInt32              GetVideoMode                () const;
    UInt32              GetUpdateFormat             () const;
    UInt32              GetMemoryFormat             () const;

    UInt32              GetReferenceMode            () const;
    UInt32              GetReferenceH               () const;
    UInt32              GetReferenceV               () const;
    bool                GetFlipped                  () const;
    bool                AutoGeneratedTimecode       () const;
    bool                TimecodePresented           () const;

    void                SetVideoOutput              ( bool enable );

    virtual UInt32      GetResolution               () const override;

    virtual void        StartThreads                () override;
    virtual void        StopThreads                 () override;
    virtual void        SuspendThreads              () override;
    virtual void        ResumeThreads               () override;

    virtual bool        IsInputChannel              () const override;
    virtual bool        IsOutputChannel             () const override;

    void                EnqueueFrame                ( const AVFrameConstPtr & frame );

private:

    void                FrameProcessed	            ( const AVFrameConstPtr & frame );

    void                UpdateFrameTime             ( UInt64 t );
    UInt64              GetFrameTime                () const;

    friend class BlueFishVCThread;
};





} //bluefish
} //videocards
} //bv
