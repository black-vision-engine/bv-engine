#include "FakeVideoCard.h"

#include "System/Path.h"
#include "LibImage.h"


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
void            FakeVideoCard::SetVideoInput    ( VideoInputID inputId, bool enable )
{
    inputId;
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
    AVFramePtr resultFrame;

    auto & frames = m_inputFrames[ inputID ];

    auto nextFrame = frames.m_nextFramePtr;
    
    if( nextFrame < frames.m_frames.size() )
    {
        resultFrame = frames.m_frames[ nextFrame ];

        frames.m_nextFramePtr = ( frames.m_nextFramePtr + 1 ) % frames.m_frames.size();
    }

    return resultFrame;
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

            AVFrameDescriptor frameDesc = CreateAVFrameDesc( channel.InputChannelData.get() );

            VideoInputChannelDesc newDesc( m_deviceID, inputID, FakeVideoCardDesc::UID(), channelName, frameDesc );
            descs.push_back( newDesc );
        }
    }

    return descs;
}

// ***********************
//
OutputChannelsDescsVec  FakeVideoCard::GetOutputChannelsDescs           () const
{
    OutputChannelsDescsVec descs;

    for( auto & channel : m_channels )
    {
        if( channel.OutputChannelData != nullptr )
        {
            VideoOutputID outputID = channel.OutputChannelData->LinkedVideoOutput;
            const std::string channelName = channel.Name;

            AVFrameDescriptor frameDesc = CreateAVFrameDesc( channel.OutputChannelData.get() );

            VideoOutputChannelDesc newDesc( m_deviceID, outputID, FakeVideoCardDesc::UID(), channelName, frameDesc );
            descs.push_back( newDesc );
        }
    }

    return descs;
}

// ***********************
//
ReturnResult            FakeVideoCard::SetReferenceMode                 ( ReferenceMode /*mode*/ )
{
    return Result::Failure();
}

// ***********************
//
ReturnResult            FakeVideoCard::SetReferenceH                    ( UInt32 /*offsetH*/ )
{
    return Result::Failure();
}

// ***********************
//
ReturnResult            FakeVideoCard::SetReferenceV                    ( UInt32 /*offsetV*/ )
{
    return Result::Failure();
}

// ***********************
//
ReferenceInfo           FakeVideoCard::GetReferenceInfo                 () const
{
    return ReferenceInfo();
}

// ***********************
//
bool                    FakeVideoCard::IsLocked                         () const
{
    return false;
}

// ***********************
//
void                    FakeVideoCard::AddChannel                       ( FakeChannelDesc & channelDesc )
{
    if( channelDesc.InputChannelData )
    {
        LoadInputChannelFrames( *( channelDesc.InputChannelData.get() ) );
    }
    
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

// ***********************
//
void                    FakeVideoCard::ResetInputFrame                  ( VideoInputID id )
{
    m_inputFrames[ id ].m_nextFramePtr = 0;
}

// ***********************
//
bool                    FakeVideoCard::IsEnabled                        ( VideoInputID inputId ) const
{
    for( auto & channel : m_channels )
    {
        if( channel.InputChannelData && channel.InputChannelData->LinkedVideoInput == inputId )
        {
            return channel.InputChannelData->Enabled;
        }
    }

    return false;
}

// ***********************
//
void                    FakeVideoCard::LoadInputChannelFrames           ( const FakeInputChannelData & channelDesc )
{
    FakeInputFrames inputFrames;
    Path imagesPath = channelDesc.ImagesDirectory;
    Path audioPath = channelDesc.AudioFile;

    MemoryChunkVector audioFrames;

    if( Path::Exists( audioPath ) )
    {
        audioFrames = LoadAudio( audioPath );
    }

    if( Path::Exists( imagesPath ) )
    {
        SizeType frameIdx = 0;

        auto fileList = Path::List( imagesPath, false );
        for( auto & file : fileList )
        {
            auto imageChunk = LoadImage( file, channelDesc.Width, channelDesc.Height );
            if( imageChunk )
            {
                auto frame = AVFrame::Create();
                frame->m_audioData = audioFrames.size() > frameIdx ? audioFrames[ frameIdx ] : nullptr;
                frame->m_videoData = imageChunk;
                frame->m_desc = CreateAVFrameDesc( &channelDesc );

                inputFrames.m_frames.push_back( frame );

                frameIdx++;
            }
        }
    }

    m_inputFrames[ channelDesc.LinkedVideoInput ] = inputFrames;
}

// ***********************
//
MemoryChunkPtr          FakeVideoCard::LoadImage        ( const Path & imagePath, UInt32 expectedWidth, UInt32 expectedHeight )
{
    UInt32 width;
    UInt32 height;
    UInt32 bpp;
    UInt32 channelNum;

    auto data = image::LoadImageImpl( imagePath.Str(), &width, &height, &bpp, &channelNum );
    auto resized = image::ResizeImpl( data, width, height, bpp, expectedWidth, expectedHeight, image::FilterType::FT_BOX );

    delete[] data;

    return MemoryChunk::Create( resized, expectedWidth * expectedHeight * bpp / 8 );
}

// ***********************
//
MemoryChunkVector       FakeVideoCard::LoadAudio                ( const Path & audioPath )
{
    MemoryChunkVector audioFrames;

    auto sampleSize = sizeof( UInt16 );
    auto numChannels = 2;
    auto frequency = 48000;
    auto fps = 50;
    
    auto samplesPerFrame = numChannels * frequency / fps;
    auto frameSize = samplesPerFrame * sampleSize;

    auto file = fopen( audioPath.Str().c_str(), "rb" );
    if( file )
    {
        SizeType readBytes = 0;
        do
        {
            auto frameChunk = MemoryChunk::Create( frameSize );
            readBytes = fread( frameChunk->GetWritable(), 1, frameSize, file );

            audioFrames.push_back( frameChunk );

        } while( readBytes == frameSize );

        fclose( file );
    }

    return audioFrames;
}

// ***********************
//
AVFrameDescriptor       FakeVideoCard::CreateAVFrameDesc        ( const FakeInputChannelData * channelDesc ) const
{
    AVFrameDescriptor frameDesc;

    frameDesc.height = channelDesc->Height;
    frameDesc.width = channelDesc->Width;
    frameDesc.depth = 4;

    return frameDesc;
}

// ***********************
//
AVFrameDescriptor       FakeVideoCard::CreateAVFrameDesc        ( const FakeOutputChannelData * channelDesc ) const
{
    AVFrameDescriptor frameDesc;

    frameDesc.height = channelDesc->Height;
    frameDesc.width = channelDesc->Width;
    frameDesc.depth = 4;

    return frameDesc;
}


}   // videocards
}   // bv


