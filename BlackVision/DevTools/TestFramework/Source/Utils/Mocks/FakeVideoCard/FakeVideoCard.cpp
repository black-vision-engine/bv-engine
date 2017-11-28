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
    return m_deviceID;
}

// ***********************
//
void            FakeVideoCard::ProcessFrame     ( const AVFrameConstPtr & data, UInt64 avOutputID )
{
    m_lastFrameOutput.Outputs[ avOutputID ] = data;
}

// ***********************
//
AVFramePtr      FakeVideoCard::QueryInputFrame  ( VideoInputID inputID )
{
    // @todo Make mechanism to provide frames from outside in test, which will be queried here.

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
    return 50;
}

// ***********************
//
std::set< UInt64 >      FakeVideoCard::GetDisplayedVideoOutputsIDs      () const
{
    std::set< UInt64 > ret;

    for( auto & channel : m_channels )
    {
        if( channel.OutputChannelData )
            ret.insert( channel.OutputChannelData->LinkedVideoOutput );
    }

    return ret;
}

// ***********************
//
InputChannelsDescsVec   FakeVideoCard::GetInputChannelsDescs            () const
{
    InputChannelsDescsVec descs;

    for( auto & channel : m_channels )
    {
        if( channel.InputChannelData != nullptr )
        {
            VideoInputID inputID = channel.InputChannelData->LinkedVideoInput;
            const std::string channelName = channel.Name;

            AVFrameDescriptor frameDesc;

            frameDesc.height = channel.InputChannelData->Height;
            frameDesc.width = channel.InputChannelData->Width;
            frameDesc.depth = 4;

            VideoInputChannelDesc newDesc( m_deviceID, inputID, FakeVideoCardDesc::UID(), channelName, frameDesc );
            descs.push_back( newDesc );
        }
    }

    return descs;
}

// ***********************
//
void                    FakeVideoCard::AddChannel                       ( FakeChannelDesc & channelDesc )
{
    m_channels.push_back( std::move( channelDesc ) );
}

// ***********************
//
void                    FakeVideoCard::ClearOutputs                     ()
{
    m_lastFrameOutput.Outputs.clear();
}




}   // videocards
}   // bv


