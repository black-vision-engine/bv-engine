#pragma once


#include "Interfaces/IVideoCard.h"


namespace bv {
namespace videocards
{

// ***********************
//
class FakeVideoCard : public IVideoCard
{
private:

public:

    virtual                     ~FakeVideoCard          () {}

    virtual void                PreStart                () override;
    virtual void                Start                   () override;
    virtual void                Stop                    () override;

    virtual void                SetVideoOutput          ( bool enable ) override;

    virtual VideoCardID         GetVideoCardID          () const override;

    //TODO: handle frames from GPU
    virtual void                ProcessFrame            ( const AVFrameConstPtr & data, UInt64 avOutputID ) override;
    virtual AVFramePtr          QueryInputFrame         ( VideoInputID inputID ) override;

    virtual void                EnableAudioChannel      ( AudioSampleType audioSampleType, UInt32 sampleRate, UInt32 channelCount ) override;

    virtual void                SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType callback ) override;

    virtual UInt32                      GetRequiredFPS                  () const override;

    virtual std::set< UInt64 >	        GetDisplayedVideoOutputsIDs     () const override;
    virtual InputChannelsDescsVec       GetInputChannelsDescs           () const override;
};

DEFINE_PTR_TYPE( FakeVideoCard )
DEFINE_CONST_PTR_TYPE( FakeVideoCard )


}   // videocards
}   // bv
