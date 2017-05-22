#include "BlueFishVideoCard.h"

#include "Serialization/SerializationHelper.h"

#include "Mathematics/Core/mathfuncs.h"

#include "UseLoggerVideoModule.h"

namespace bv { namespace videocards { namespace bluefish {


//**************************************
//
                        VideoCardDesc::VideoCardDesc    ()
    : m_uid( "BlueFish" )
{
}

//**************************************
//
IVideoCardPtr           VideoCardDesc::CreateVideoCard          ( const IDeserializer & deser ) const
{
    auto deviceID = 0;
    if( deser.EnterChild( "deviceID" ) )
    {
        deviceID = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "value" ), 0 );
        
        deser.ExitChild(); //deviceID
    }

    auto card = std::make_shared< VideoCard >( deviceID );

    if( card->AttachVideoCard() )
    {
        //check input / output count
        if( deser.EnterChild( "channels" ) )
        {
            if( deser.EnterChild( "channel" ) )
            {
                do
                {
                    ChannelInputDataUPtr input = nullptr;
                    ChannelOutputDataUPtr output = nullptr;

                    auto name = SerializationHelper::String2T< ChannelName >( deser.GetAttribute( "name" ) );

                    if( deser.EnterChild( "input" ) )
                    {
                        input = std::unique_ptr< ChannelInputData >( new ChannelInputData() );
                        input->type = SerializationHelper::String2T< IOType >( deser.GetAttribute( "type" ) );
                        input->playthrough = SerializationHelper::String2T< bool >( deser.GetAttribute( "playthrough" ), true );
                    
                        deser.ExitChild(); //input
                    }

                    if( deser.EnterChild( "output" ) )
                    {
                        output = std::unique_ptr< ChannelOutputData >( new ChannelOutputData() );
                        output->type = SerializationHelper::String2T< IOType >( deser.GetAttribute( "type" ) );
                        output->resolution = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "resolution" ), 1080 );
                        output->refresh = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "refresh" ), 5000 );
                        output->interlaced = SerializationHelper::String2T< bool >( deser.GetAttribute( "interlaced" ), false );
                        output->flipped = SerializationHelper::String2T< bool >( deser.GetAttribute( "flipped" ), false );
                        output->referenceMode = ReferenceModeMap[ SerializationHelper::String2T< ReferenceMode >( deser.GetAttribute( "referenceMode" ) ) ];
                        output->referenceH = SerializationHelper::String2T< Int32 >( deser.GetAttribute( "referenceH" ), 0 );
                        output->referenceV = SerializationHelper::String2T< Int32 >( deser.GetAttribute( "referenceV" ), 0 );
                        output->videoMode = ConvertVideoMode( output->resolution, output->refresh, output->interlaced );
                        output->linkedVideoOutput = ( UInt32 ) SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "linkedVideoOutput" ), 0 );

                        //FIXME?
                        output->updateFormat = UPD_FMT_FIELD;
                        output->memoryFormat = MEM_FMT_BGRA;

                        deser.ExitChild(); //output
                    }

                    card->AddChannel( new Channel( name, input, output ) );
                    
                } while( deser.NextChild() );

                deser.ExitChild(); //channel
            }

            deser.ExitChild(); //channels
        }

        card->InitVideoCard();
        return card;
    }

    return nullptr;
}

//**************************************
//
const std::string &     VideoCardDesc::GetVideoCardUID      () const
{
    return m_uid;
}

//**************************************
//
VideoCard::VideoCard                ( UInt32 deviceID )
    : m_deviceID( deviceID )
    , m_engineMode( VIDEO_ENGINE_DUPLEX )
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
std::set< UInt64 >	VideoCard::GetDisplayedVideoOutputsIDs() const
{
    std::set< UInt64 > ret;

    for( auto ch : m_channels )
        ret.insert( ch->GetOutputId() );

    return ret;
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

    SetVideoOutput( false );

	for( auto & channel : m_channels )
	{
        auto captureChannel = channel->GetCaptureChannel();
        if( captureChannel )
		{
            if( channel->GetInputType() != IOType::FILL_KEY )
            {
                captureChannel->Init( m_deviceID, channel->GetInputChannel(), channel->GetUpdateFormat(), channel->GetMemoryFormat(), channel->GetCaptureBuffer() );
				captureChannel->RouteChannel( channel->GetEpochSDIInput(), channel->GetEpochInputMemInterface(), BLUE_CONNECTOR_PROP_SINGLE_LINK );
            }
            else
            {
                captureChannel->InitDualLink( m_deviceID, channel->GetInputChannel(), channel->GetUpdateFormat(), channel->GetMemoryFormat(), channel->GetCaptureBuffer() );
            }

			captureChannel->InitThread();
		}

        auto playbackChannel = channel->GetPlaybackChannel();
        if( playbackChannel )
		{
            playbackChannel->Init( m_deviceID, channel->GetOutputChannel(), channel->GetUpdateFormat(), channel->GetMemoryFormat(), channel->GetVideoMode(), 
            channel->GetPlaybackBuffer(), channel->GetReferenceMode(), channel->GetReferenceH(), channel->GetReferenceV(), channel->GetFlipped(),true,true, EPOCH_DEST_SDI_OUTPUT_A);

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

            playbackChannel->InitThread();
            //FIXME: playback_counter / playbackChannel->InitThread(); 
		}			
	}

    SetVideoOutput( true );
}

//**************************************
//
void            VideoCard::SetVideoOutput        ( bool enable )
{
    for( auto channel : m_channels )
    {
        channel->SetVideoOutput( enable );
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
    for( auto channel : m_channels )
    {
        if( channel->GetOutputId() == outputId )
        {
            channel->EnqueueFrame( frame );
        }
	}   
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
    UInt32 fps = 1;

    for( auto & ch : m_channels )
    {
        if( ch->m_playbackData )
            fps = mathematics::lcm( ch->m_playbackData->refresh, fps );
    }
 
    return fps / 100;
}

} //bluefish
} //videovcards
} //bv
