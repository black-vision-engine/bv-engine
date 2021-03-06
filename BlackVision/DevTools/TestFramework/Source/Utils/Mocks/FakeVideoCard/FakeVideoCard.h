#pragma once


#include "Interfaces/IVideoCard.h"
#include "FakeVideoCardDesc.h"
#include "FakeInputFrames.h"

#include "System/Path.h"

#include <map>
#include <condition_variable>
#include <mutex>
#include <atomic>


namespace bv {
namespace videocards
{


struct FrameOutput
{
    std::map< UInt64, AVFrameConstPtr >     Outputs;
};


// ***********************
//
class FakeVideoCard : public IVideoCard
{
private:

    UInt32                          m_deviceID;
    std::vector< FakeChannelDesc >  m_channels;

    FrameOutput                     m_lastFrameOutput;

    // Output frames synchronization
    std::atomic< UInt64 >           m_frameNum;
    UInt64                          m_syncFrame;

    std::mutex                      m_syncLock;
    std::condition_variable         m_waitForFrame;

    // Inputs
    std::map< VideoInputID, FakeInputFrames >       m_inputFrames;

public:

    explicit                    FakeVideoCard           ( UInt32 deviceID );
    virtual                     ~FakeVideoCard          () {}

    virtual void                PreStart                () override;
    virtual void                Start                   () override;
    virtual void                Stop                    () override;

    virtual void                SetVideoOutput          ( bool enable ) override;
    virtual void                SetVideoInput           ( VideoInputID inputId, bool enable ) override;

    virtual VideoCardID         GetVideoCardID          () const override;

    //TODO: handle frames from GPU
    virtual void                ProcessFrame            ( const AVFrameConstPtr & data, UInt64 avOutputID ) override;
    virtual AVFramePtr          QueryInputFrame         ( VideoInputID inputID ) override;

    virtual void                EnableAudioChannel      ( AudioSampleType audioSampleType, UInt32 sampleRate, UInt32 channelCount ) override;

    virtual void                SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType callback ) override;

    virtual UInt32                      GetRequiredFPS                  () const override;

    virtual std::set< UInt64 >	        GetDisplayedVideoOutputsIDs     () const override;
    virtual InputChannelsDescsVec       GetInputChannelsDescs           () const override;
    virtual OutputChannelsDescsVec      GetOutputChannelsDescs          () const override;

    virtual ReturnResult        SetReferenceMode        ( ReferenceMode mode ) override;

    virtual ReturnResult        SetReferenceH           ( UInt32 offsetH ) override;
    virtual ReturnResult        SetReferenceV           ( UInt32 offsetV ) override;

    virtual ReferenceInfo       GetReferenceInfo        () const override;
    virtual bool                IsLocked                () const override;

public:

    void                        AddChannel              ( FakeChannelDesc & channelDesc );

    void                        SetOutputSyncPoint      ();

    FrameOutput &               AccessOutputs           ();
    void                        ClearOutputs            ();

    void                        ResetInputFrame         ( VideoInputID id );

    bool                        IsEnabled               ( VideoInputID inputId ) const;

private:

    void                        LoadInputChannelFrames  ( const FakeInputChannelData & channelDesc );
    MemoryChunkPtr              LoadImage               ( const Path & imagePath, UInt32 expectedWidth, UInt32 expectedHeight );
    MemoryChunkVector           LoadAudio               ( const Path & audioPath );

    AVFrameDescriptor           CreateAVFrameDesc       ( const FakeInputChannelData * channelDesc ) const;
    AVFrameDescriptor           CreateAVFrameDesc       ( const FakeOutputChannelData * channelDesc ) const;
};

DEFINE_PTR_TYPE( FakeVideoCard )
DEFINE_CONST_PTR_TYPE( FakeVideoCard )


}   // videocards
}   // bv
