#include "BlueFishVideoCard.h"

#include "Serialization/SerializationHelper.h"


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

    m_SDK->device_detach();
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

    //FIXME?
	//ToDo unhack nasfeter
	/*if(Channels[GetChannelByName("A")]->GetType()=="FILL_KEY") 
	{
		Channels.erase(Channels.begin()+GetChannelByName("B"));
		cout << "YOU CANNOT USE CHANNEL B WHEN CHANNEL A TYPE IS FILL_KEY" << endl;
		system("pause");
		//return 0;
	}

	if(Channels[GetChannelByName("C")]->GetType()=="FILL_KEY") 
	{
		Channels.erase(Channels.begin()+GetChannelByName("D"));
		cout << "YOU CANNOT USE CHANNEL D WHEN CHANNEL C TYPE IS FILL_KEY" << endl;
		system("pause");
		//return 0;
	}

	if(Channels[GetChannelByName("E")]->GetType()=="FILL_KEY") 
	{
		Channels.erase(Channels.begin()+GetChannelByName("F"));
		cout << "YOU CANNOT USE CHANNEL F WHEN CHANNEL E TYPE IS FILL_KEY" << endl;
		system("pause");
		//return 0;
	}*/
	////

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
                channel->GetPlaybackBuffer(), channel->GetReferenceMode(), channel->GetReferenceH(), channel->GetReferenceV(), channel->GetFlipped() );

			if( channel->GetOutputType() == IOType::FILL || channel->GetOutputType() == IOType::KEY )
			{
				playbackChannel->RouteChannel( channel->GetEpochOutputMemInterface(), channel->GetEpochSDIOutput(), BLUE_CONNECTOR_PROP_SINGLE_LINK );
			}
			else if( channel->GetOutputType() == IOType::FILL_KEY )
			{						
				playbackChannel->RouteChannel( channel->GetEpochOutputMemInterface(), channel->GetEpochSDIOutput(), BLUE_CONNECTOR_PROP_DUALLINK_LINK_1 );
				playbackChannel->RouteChannel( channel->GetEpochOutputMemInterface(), channel->GetEpochSDIOutput(), BLUE_CONNECTOR_PROP_DUALLINK_LINK_2 );
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
void                            VideoCard::Start                    ()
{
    for( auto channel : m_channels )
	{
		channel->StartThreads();
	}
}

//**************************************
//
void                            VideoCard::ProcessFrame             ( MemoryChunkConstPtr data )
{
    for( auto channel : m_channels )
	{
        auto playbackChannel = channel->GetPlaybackChannel();
        if( playbackChannel && !channel->PlaythroughEnabled() )
		{
            playbackChannel->m_pFifoBuffer->PushFrame( 
                std::make_shared< CFrame >( reinterpret_cast< const unsigned char * >( data->Get() ), 
                                            m_deviceID,
                                            playbackChannel->GoldenSize, 
                                            playbackChannel->BytesPerLine ) );
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

////**************************************
////
//void BlueFishVideoCard::Fill50(unsigned short* pAudio16, unsigned int Samples, unsigned int Channels)
//{
//    unsigned short sine_wave_const_50[] = { 0x0000, 0x0197, 0x0327, 0x04AC, 0x061D, 0x0775,
//        0x08B0, 0x09C7, 0x0AB7, 0x0B7B, 0x0C11, 0x0C77,
//        0x0CAA, 0x0CAA, 0x0C77, 0x0C11, 0x0B7B, 0x0AB7,
//        0x09C7, 0x08B0, 0x0775, 0x061D, 0x04AC, 0x0327,
//        0x0197, 0x0000, 0xFE68, 0xFCD8, 0xFB53, 0xF9E2,
//        0xF88A, 0xF74F, 0xF638, 0xF548, 0xF484, 0xF3EE,
//        0xF388, 0xF355, 0xF355, 0xF388, 0xF3EE, 0xF484,
//        0xF548, 0xF638, 0xF74F, 0xF88A, 0xF9E2, 0xFB53,
//        0xFCD8, 0xFE68 };
//    unsigned short data = 0;
//    for(unsigned short i=0; i<Samples/50; i++)
//    {
//        for(unsigned short k=0; k<50; k++)
//        {
//            data = sine_wave_const_50[k];
//            // 16 channels
//            if(Channels > 0)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 1)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 2)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 3)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 4)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 5)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 6)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 7)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 8)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 9)  { *pAudio16 = data; pAudio16++; }
//            if(Channels > 10) { *pAudio16 = data; pAudio16++; }
//            if(Channels > 11) { *pAudio16 = data; pAudio16++; }
//            if(Channels > 12) { *pAudio16 = data; pAudio16++; }
//            if(Channels > 13) { *pAudio16 = data; pAudio16++; }
//            if(Channels > 14) { *pAudio16 = data; pAudio16++; }
//            if(Channels > 15) { *pAudio16 = data; pAudio16++; }
//        }
//    }
//}
//
////**************************************
////
//void BlueFishVideoCard::processAudioOutput(unsigned int nCardType, void* pAudioOut, unsigned int VideoMode)
//{
//    if(!pAudioOut)
//    {
//        cout << "AUDIO ERROR: Out: " << (unsigned int)pAudioOut << endl; 
//        return;
//    }
//
//    hanc_stream_info_struct HancEncodeStruct;
//    memset(&HancEncodeStruct, 0, sizeof(HancEncodeStruct));
//
//    //encode audio
//    HancEncodeStruct.AudioDBNArray[0] = -1;
//    HancEncodeStruct.AudioDBNArray[1] = -1;
//    HancEncodeStruct.AudioDBNArray[2] = -1;
//    HancEncodeStruct.AudioDBNArray[3] = -1;
//    HancEncodeStruct.hanc_data_ptr = (unsigned int*)pAudioOut;
//    HancEncodeStruct.video_mode = VideoMode;
//
//    encode_hanc_frame_ex(nCardType, &HancEncodeStruct, (void*)g_pAudioScratch, 2, 1600, AUDIO_CHANNEL_16BIT, blue_emb_audio_enable | blue_emb_audio_group1_enable);
//}
////**************************************
////
//bool	VideoCard::CheckIfNewFrameArrived( ChannelName channelName )
//{
//    auto channel = GetChannelByName( channelName );
//    if( channel && channel->GetCaptureBuffer() )
//    {
//        return !channel->GetCaptureBuffer()->IsLiveBufferEmpty();
//    }
//    return false;
//}
////**************************************
////
//unsigned char *	VideoCard::GetCaptureBufferForShaderProccessing( ChannelName channelName )
//{
//    auto channel = GetChannelByName( channelName );
//    if( channel && channel->GetCaptureChannel() )
//    {
//        return channel->GetCaptureChannel()->m_pFifoBuffer->GetLiveBuffer()->m_pBuffer;
//    }
//    return nullptr;
//}

} //bluefish
} //videovcards
} //bv
