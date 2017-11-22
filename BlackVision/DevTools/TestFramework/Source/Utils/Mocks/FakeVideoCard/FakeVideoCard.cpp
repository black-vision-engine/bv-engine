#include "FakeVideoCard.h"




namespace bv {
namespace videocards
{

// ***********************
//
FakeVideoCard::FakeVideoCard( UInt32 deviceID )
    :   m_deviceID( deviceID )
{}

// ***********************
//
void            FakeVideoCard::PreStart     ()
{}

// ***********************
//
void            FakeVideoCard::Start        ()
{}

// ***********************
//
void            FakeVideoCard::Stop         ()
{}

// ***********************
//
void            FakeVideoCard::SetVideoOutput   ( bool enable )
{
    enable;
}

// ***********************
//
VideoCardID     FakeVideoCard::GetVideoCardID   () const
{
    return VideoCardID();
}

// ***********************
//
void            FakeVideoCard::ProcessFrame     ( const AVFrameConstPtr & data, UInt64 avOutputID )
{
    data;
    avOutputID;
}

// ***********************
//
AVFramePtr      FakeVideoCard::QueryInputFrame  ( VideoInputID inputID )
{
    inputID;
    return AVFramePtr();
}

// ***********************
//
void            FakeVideoCard::EnableAudioChannel   ( AudioSampleType audioSampleType, UInt32 sampleRate, UInt32 channelCount )
{
    audioSampleType;
    sampleRate;
    channelCount;
}

// ***********************
//
void            FakeVideoCard::SetFrameProcessingCompletedCallback  ( FrameProcessingCompletedCallbackType callback )
{
    callback;
}

// ***********************
//
UInt32                  FakeVideoCard::GetRequiredFPS                   () const
{
    return UInt32();
}

// ***********************
//
std::set< UInt64 >      FakeVideoCard::GetDisplayedVideoOutputsIDs      () const
{
    return std::set<UInt64>();
}

// ***********************
//
InputChannelsDescsVec   FakeVideoCard::GetInputChannelsDescs            () const
{
    return InputChannelsDescsVec();
}

// ***********************
//
void                    FakeVideoCard::AddChannel                       ( FakeChannelDesc & channelDesc )
{
    m_channels.push_back( std::move( channelDesc ) );
}



}   // videocards
}   // bv


