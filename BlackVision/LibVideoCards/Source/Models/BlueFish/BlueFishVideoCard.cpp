#include "BlueFishVideoCard.h"

#include "Serialization/SerializationHelper.h"

#include "Mathematics/Core/mathfuncs.h"

#include "UseLoggerVideoModule.h"

namespace bv {
namespace videocards {
namespace bluefish
{




//**************************************
//
VideoCard::VideoCard                ( UInt32 deviceID, ReferenceMode mode, UInt32 referenceH, UInt32 referenceV )
    : m_deviceID( deviceID )
    , m_engineMode( VIDEO_ENGINE_DUPLEX )
    , m_referenceMode( mode )
    , m_referenceH( referenceH )
    , m_referenceV( referenceV )
{
    m_SDK = CBlueVelvet4Ptr( BlueVelvetFactory4() );
}

//**************************************
//
VideoCard::~VideoCard               ()
{
	for( auto & channel: m_channels )
    {
        delete channel;
    }
    m_channels.clear();

    if( !DetachVideoCard() )
        LOG_MESSAGE( SeverityLevel::error ) << "Cannot properly detach device: " << m_deviceID;
}

//**************************************
//
std::set< UInt64 >	    VideoCard::GetDisplayedVideoOutputsIDs  () const
{
    std::set< UInt64 > ret;

    for( auto ch : m_channels )
    {
        if( ch->IsOutputChannel() )
            ret.insert( static_cast< OutputChannel * >( ch )->GetOutputId() );
    }

    return ret;
}

// ***********************
//
InputChannelsDescsVec   VideoCard::GetInputChannelsDescs        () const
{
    InputChannelsDescsVec descs;

    for( auto channel : m_channels )
    {
        if( channel->IsInputChannel() )
        {
            auto typedChannel = static_cast< InputChannel * >( channel );

            VideoInputID inputID = typedChannel->GetInputId();
            const std::string channelName = Convert::T2String( channel->GetName() );
            
            AVFrameDescriptor frameDesc = channel->CreateFrameDesc();

            VideoInputChannelDesc newDesc( m_deviceID, inputID, VideoCardDesc::UID(), channelName, frameDesc );
            descs.push_back( newDesc );
        }
    }

    return descs;
}

// ***********************
//
OutputChannelsDescsVec  VideoCard::GetOutputChannelsDescs       () const
{
    OutputChannelsDescsVec descs;

    for( auto channel : m_channels )
    {
        if( channel->IsOutputChannel() )
        {
            auto typedChannel = static_cast< OutputChannel * >( channel );

            VideoOutputID outputID = typedChannel->GetOutputId();
            const std::string channelName = Convert::T2String( channel->GetName() );

            AVFrameDescriptor frameDesc = channel->CreateFrameDesc();

            VideoOutputChannelDesc newDesc( m_deviceID, outputID, VideoCardDesc::UID(), channelName, frameDesc );
            descs.push_back( newDesc );
        }
    }

    return descs;
}

//**************************************
//
bool            VideoCard::DetachVideoCard          ()
{
    return BLUE_OK( m_SDK->device_detach() );
}

//**************************************
//
bool            VideoCard::AttachVideoCard          ()
{
    return BLUE_OK( m_SDK->device_attach( m_deviceID, FALSE ) );
}

//**************************************
//
void            VideoCard::InitVideoCard            ()
{
    varVal.ulVal = m_engineMode;
    m_SDK->SetCardProperty( VIDEO_OUTPUT_ENGINE, varVal );

    SetReferenceMode( m_referenceMode );
    SetReference( m_referenceH, m_referenceV );

    SetVideoOutput( false );

	for( auto channelIter = m_channels.begin(); channelIter != m_channels.end(); )
	{
        if( (*channelIter)->IsInputChannel() )
		{
            auto channel = static_cast< InputChannel * >( ( *channelIter ) );
            auto captureChannel = channel->GetCaptureChannel();

            ReturnResult result;

            if( channel->GetInputType() != IOType::FILL_KEY )
            {
                result = captureChannel->Init( m_deviceID, channel->GetInputChannel(), channel->GetUpdateFormat(), channel->GetMemoryFormat(), channel->GetCaptureBuffer() );
                
                if( result.IsValid() )
                    captureChannel->RouteChannel( channel->GetEpochSDIInput(), channel->GetEpochInputMemInterface(), BLUE_CONNECTOR_PROP_SINGLE_LINK );
            }
            else
            {
                result = captureChannel->InitDualLink( m_deviceID, channel->GetInputChannel(), channel->GetUpdateFormat(), channel->GetMemoryFormat(), channel->GetCaptureBuffer() );
            }

            if( result.IsValid() )
                result = captureChannel->InitThread();

            if( !result.IsValid() )
            {
                LOG_MESSAGE( SeverityLevel::warning ) << "Input Channel " << Convert::T2String( ( *channelIter )->GetName() ) << " couldn't be initialize. Error: " << result.GetErrorReason();

                channelIter = m_channels.erase( channelIter );
                continue;
            }
		}
        else if( ( *channelIter )->IsOutputChannel() )
		{
            auto channel = static_cast< OutputChannel * >( ( *channelIter ) );
            auto playbackChannel = channel->GetPlaybackChannel();

            ReturnResult result;

            result = playbackChannel->Init( m_deviceID, channel->GetOutputChannel(), channel->GetUpdateFormat(), channel->GetMemoryFormat(), channel->GetVideoMode(),
            channel->GetPlaybackBuffer(), channel->GetFlipped(), true, true, EPOCH_DEST_SDI_OUTPUT_A);

            if( result.IsValid() )
            {
                if( channel->GetOutputType() == IOType::FILL )
                {
                    playbackChannel->RouteChannel( channel->GetEpochOutputMemInterface(), channel->GetEpochSDIOutput(), BLUE_CONNECTOR_PROP_SINGLE_LINK );
                }
                else if( channel->GetOutputType() == IOType::KEY )
                {
                    playbackChannel->RouteChannel( channel->GetEpochOutputMemInterface(), channel->GetEpochSDIOutput(), BLUE_CONNECTOR_PROP_DUALLINK_LINK_2 );
                }
                else if( channel->GetOutputType() == IOType::FILL_KEY )
                {
                    playbackChannel->RouteChannel( channel->GetEpochOutputMemInterface(), channel->GetEpochSDIOutput(), BLUE_CONNECTOR_PROP_DUALLINK_LINK_1 );
                    playbackChannel->RouteChannel( channel->GetEpochOutputMemInterface(), channel->GetEpochSDIKeyOutput(), BLUE_CONNECTOR_PROP_DUALLINK_LINK_2 );
                }

                result = playbackChannel->InitThread();

                playbackChannel->m_AutoGeneratedTimecode = channel->AutoGeneratedTimecode();
                playbackChannel->m_EnableTimecode = channel->TimecodePresented();
                //FIXME: playback_counter / playbackChannel->InitThread(); 
            }

            if( !result.IsValid() )
            {
                LOG_MESSAGE( SeverityLevel::warning ) << "Output Channel " << Convert::T2String( ( *channelIter )->GetName() ) << " couldn't be initialize. Error: " << result.GetErrorReason();

                channelIter = m_channels.erase( channelIter );
                continue;
            }
		}

        channelIter++;
	}

    SetVideoOutput( true );
}

//**************************************
//
void            VideoCard::SetVideoOutput        ( bool enable )
{
    for( auto channel : m_channels )
    {
        if( channel->IsOutputChannel() )
        {
            static_cast< OutputChannel * >( channel )->SetVideoOutput( enable );
        }
    }
}

// ***********************
//
void            VideoCard::SetVideoInput        ( VideoInputID inputId, bool enable )
{
    for( auto channel : m_channels )
    {
        if( channel->IsInputChannel() )
        {
            auto inputChannel = static_cast< InputChannel * >( channel );
            if( inputChannel->GetInputId() == inputId )
                inputChannel->SetVideoInput( enable );
        }
    }
}

//**************************************
//
void            VideoCard::AddChannel               ( Channel * channel )
{
    m_channels.push_back( channel );
}

//**************************************
//
void            VideoCard::RouteChannel             ( ULONG source, ULONG destination, ULONG linkType )
{
    VARIANT varVal;
    varVal.vt = VT_UI4;

    varVal.ulVal = EPOCH_SET_ROUTING( source, destination, linkType );
    m_SDK->SetCardProperty( MR2_ROUTING, varVal );
}

//**************************************
//
Channel *       VideoCard::GetChannelByName         ( ChannelName channelName ) const
{
    for( auto channel : m_channels )
	{
		if( channel->GetName() == channelName )
        {
            return channel;
        }
	}

    return nullptr;
}

//**************************************
//
void                            VideoCard::PreStart                     ()
{}


//**************************************
//
void                            VideoCard::Start                        ()
{
    for( auto channel : m_channels )
	{
		channel->StartThreads();
	}
}

//**************************************
//
void                            VideoCard::Stop                         ()
{
    for( auto channel : m_channels )
    {
        channel->StopThreads();
    }
};

//**************************************
//
void                            VideoCard::ProcessFrame             ( const AVFrameConstPtr & frame, UInt64 outputId )
{
    for( auto chn : m_channels )
    {
        if( chn->IsOutputChannel() )
        {
            auto channel = static_cast< OutputChannel * >( chn );

            if( channel->GetOutputId() == outputId )
            {
                channel->EnqueueFrame( frame );
            }
        }
	}   
}

// ***********************
//
AVFramePtr                      VideoCard::QueryInputFrame          ( VideoInputID inputID )
{
    for( auto chn : m_channels )
    {
        if( chn->IsInputChannel() )
        {
            auto channel = static_cast< InputChannel *> ( chn );

            if( channel->GetInputId() == inputID )
            {
                return channel->QueryInputFrame();
            }
        }
    }

    return AVFramePtr();
}

//**************************************
//
UInt32                          VideoCard::EnumerateDevices         ()
{
    Int32 deviceCount = 0;
    
    CBlueVelvet4Ptr SDK( BlueVelvetFactory4() );
    if( SDK ) 
    {
		SDK->device_enumerate( deviceCount );
	}

    return ( UInt32 )deviceCount;
}

//**************************************
//
UInt32                          VideoCard::GetRequiredFPS  () const
{
    UInt32 fps = 5000;

    for( auto & chn : m_channels )
    {
        if( chn->IsOutputChannel() )
        {
            auto channel = static_cast< OutputChannel * >( chn );

            if( channel->PlaybackData )
                fps = mathematics::lcm( channel->PlaybackData->refresh, fps );
        }
    }
 
    return fps / 100;
}

// ***********************
//
ReturnResult        VideoCard::SetReferenceMode     ( ReferenceMode mode )
{
    if( mode < ReferenceMode::FailMode )
    {
        auto bluefishMode = ReferenceModeMap[ mode ];
        return UpdateReferenceMode( m_SDK.get(), bluefishMode );
    }

    return "Invalid reference mode";
}

// ***********************
//
ReturnResult        VideoCard::SetReferenceH        ( UInt32 offsetH )
{
    auto result = SetReference( offsetH, m_referenceV );
    
    if( result.IsValid() )
        m_referenceH = offsetH;

    return result;
}

// ***********************
//
ReturnResult        VideoCard::SetReferenceV        ( UInt32 offsetV )
{
    auto result = SetReference( offsetV, m_referenceV );

    if( result.IsValid() )
        m_referenceV = offsetV;

    return result;
}

// ***********************
//
ReturnResult        VideoCard::SetReference         ( UInt32 offsetH, UInt32 offsetV )
{
    return UpdateReferenceOffset( m_SDK.get(), offsetH, offsetV );
}

// ***********************
//
Expected< OutputChannel * >     VideoCard::GetOutputChannel     ( VideoOutputID outID )
{
    for( auto & chn : m_channels )
    {
        if( chn->IsOutputChannel() )
        {
            auto channel = static_cast< OutputChannel * >( chn );
            
            if( channel->GetOutputId() == outID )
                return channel;
        }
    }

    return "VideoOutput [" + Convert::T2String( outID ) + "] doesn't exist.";
}

} //bluefish
} //videovcards
} //bv
