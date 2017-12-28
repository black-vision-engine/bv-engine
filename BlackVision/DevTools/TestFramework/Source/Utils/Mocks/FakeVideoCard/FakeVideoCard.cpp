#include "FakeVideoCard.h"




namespace bv {
namespace videocards
{

// ***********************
//
FakeVideoCard::FakeVideoCard( UInt32 deviceID )
    :   m_deviceID( deviceID )
    ,   m_frameNum( 0 )
    ,   m_syncFrame( 0 )
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
    std::lock_guard< std::mutex > guard( m_syncLock );

    m_lastFrameOutput.Outputs[ avOutputID ] = data;
    m_frameNum++;

    m_waitForFrame.notify_all();
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
void                    FakeVideoCard::SetOutputSyncPoint               ()
{
    m_syncFrame = m_frameNum + 1;
}

// ***********************
//
FrameOutput &           FakeVideoCard::AccessOutputs                    ()
{
    std::unique_lock< std::mutex > lock( m_syncLock );

    // Wait until video card thread will insert new frame. You must call SetOutputSyncPoint
    // before render step to remember frame number, you want to wait.
    m_waitForFrame.wait( lock, [ this ]{ return m_frameNum >= m_syncFrame; } );

    return m_lastFrameOutput;
}

// ***********************
//
void                    FakeVideoCard::ClearOutputs                     ()
{
    m_lastFrameOutput.Outputs.clear();
}




}   // videocards
}   // bv


