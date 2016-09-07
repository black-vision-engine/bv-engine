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
    if( VideoCard::AvailableVideoCards > 0 )
    {
        auto deviceID = 0;
        if( deser.EnterChild( "deviceID" ) )
        {
            deviceID = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "value" ), 0 );
        
            deser.ExitChild(); //deviceID
        }

        auto card = std::make_shared< VideoCard >( deviceID );

        //check input / output count

        if( deser.EnterChild( "channels" ) )
        {
            if( deser.EnterChild( "channel" ) )
            {
                do
                {
                    Channel::InputDataUPtr input = nullptr;
                    Channel::OutputDataUPtr output = nullptr;

                    auto name = SerializationHelper::String2T< ChannelName >( deser.GetAttribute( "name" ) );
                    //auto renderer = deser.GetAttribute( "renderer" );

                    if( deser.EnterChild( "input" ) )
                    {
                        input = std::unique_ptr< Channel::InputData >( new Channel::InputData() );
                        input->type = SerializationHelper::String2T< IOType >( deser.GetAttribute( "type" ) );
                        input->playthrough = SerializationHelper::String2T< bool >( deser.GetAttribute( "playthrough" ), true );
                    
                        deser.ExitChild(); //input
                    }

                    if( deser.EnterChild( "output" ) )
                    {
                        output = std::unique_ptr< Channel::OutputData >( new Channel::OutputData() );
                        output->type = SerializationHelper::String2T< IOType >( deser.GetAttribute( "type" ) );
                        output->resolution = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "resolution" ), 1080 );
                        output->refresh = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "refresh" ), 5000 );
                        output->interlaced = SerializationHelper::String2T< bool >( deser.GetAttribute( "interlaced" ), false );
                        output->flipped = SerializationHelper::String2T< bool >( deser.GetAttribute( "flipped" ), false );
                        output->referenceMode = ReferenceModeMap[ SerializationHelper::String2T< ReferenceMode >( deser.GetAttribute( "referenceMode" ) ) ];
                        output->referenceH = SerializationHelper::String2T< Int32 >( deser.GetAttribute( "referenceH" ), -1 );
                        output->referenceV = SerializationHelper::String2T< Int32 >( deser.GetAttribute( "referenceV" ), -1 );
                        output->videoMode = ConvertVideoMode( output->resolution, output->refresh, output->interlaced );

                        deser.ExitChild(); //output
                    }

                    card->AddChannel( new Channel( name, input, output ) );

                } while( deser.NextChild() );

                deser.ExitChild(); //channel
            }

            deser.ExitChild(); //channels
        }

        VideoCard::AvailableVideoCards--;

        return card;
    }

    return nullptr;
}

//**************************************
//
const std::string &     VideoCardDesc::GetVideoCardUID() const
{
    return m_uid;
}


//**************************************
//
UInt32                          VideoCard::AvailableVideoCards = EnumerateDevices();
VideoCard::ChannelOptionMap     VideoCard::ChannelOptions = CreateChannelOptionMap();

//**************************************
//
VideoCard::VideoCard        ( UInt32 deviceID )
{
    m_SDK = CBlueVelvet4Ptr( BlueVelvetFactory4() );
    m_deviceID = deviceID;

    InitVideoCard();

    //m_referenceMode = BlueFreeRunning;
    //m_refH = 0;
    //m_refV = 0;
}

//**************************************
//
VideoCard::~VideoCard()
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
bool            VideoCard::InitVideoCard            ()
{
    if( BLUE_FAIL( m_SDK->device_attach( m_deviceID, FALSE ) ) )
    {
        //std::cout << "Error on device attach Device ID: " << m_deviceID << std::endl;
        return false;
    }

    m_updFmt = UPD_FMT_FRAME;
    m_memFmt = MEM_FMT_BGRA;
    m_engineMode = VIDEO_ENGINE_DUPLEX;

    SetVideoOutput( false );

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

	//unsigned int playback_counter = 0;
	for( auto & channel : m_channels )
	{
        auto captureChannel = channel->GetCaptureChannel();
        if( captureChannel )
		{
            if( channel->GetInputType() != IOType::FILL_KEY )
            {
                captureChannel->Init( m_deviceID, ChannelOptions[ channel->GetName() ].InputChannel, UPD_FMT_FRAME, MEM_FMT_BGRA, channel->GetCaptureBuffer() );
				captureChannel->RouteChannel( ChannelOptions[ channel->GetName() ].EpochSDIInput, ChannelOptions[ channel->GetName() ].EpochInputMemInterface, BLUE_CONNECTOR_PROP_SINGLE_LINK );
            }
            else
            {
                captureChannel->InitDualLink( m_deviceID, ChannelOptions[ channel->GetName() ].InputChannel, UPD_FMT_FRAME, MEM_FMT_BGRA, channel->GetCaptureBuffer() );
            }

            //channel->GetCaptureChannel()->m_playthrough = channel->m_playthrough;

			captureChannel->InitThread();
		}

        auto playbackChannel = channel->GetPlaybackChannel();
        if( playbackChannel )
		{
            playbackChannel->Init( m_deviceID, ChannelOptions[ channel->GetName() ].OutputChannel, UPD_FMT_FRAME, MEM_FMT_BGRA, channel->GetVideoMode(), 
                channel->GetPlaybackBuffer(), channel->GetReferenceMode(), channel->GetReferenceH(), channel->GetReferenceV(), channel->GetFlipped() );

			if( channel->GetInputType() == IOType::FILL || channel->GetInputType() == IOType::KEY )
			{
				playbackChannel->RouteChannel( ChannelOptions[ channel->GetName() ].EpochOutputMemInterface, ChannelOptions[ channel->GetName() ].EpochSDIOutput, BLUE_CONNECTOR_PROP_SINGLE_LINK );
			}
			else if( channel->GetInputType() == IOType::FILL_KEY )
			{						
				playbackChannel->RouteChannel( ChannelOptions[ channel->GetName() ].EpochOutputMemInterface, ChannelOptions[ channel->GetName() ].EpochSDIOutput, BLUE_CONNECTOR_PROP_DUALLINK_LINK_1 );
				playbackChannel->RouteChannel( ChannelOptions[ channel->GetName() ].EpochOutputMemInterface, ChannelOptions[ channel->GetName() ].EpochSDIOutput, BLUE_CONNECTOR_PROP_DUALLINK_LINK_2 );
			}

            /*if(channel->m_playthrough)
            {
                if(channel->GetPlaybackChannel()->InitThread())
			    {
				    std::cout << "Error on Playback InitThread A" << std::endl;
				    system("pause");
				    return 0;
			    }
            }*/

            playbackChannel->InitThread();

       //     {
       //         if(playback_counter > 0)
       //         {
       //             if(channel->GetPlaybackChannel()->InitNotSyncedThread())
			    //    {
				   //     std::cout << "Error on Playback InitThread A" << std::endl;
				   //     system("pause");
				   //     return 0;
			    //    }
       //         }
       //         else
       //         {
			    //    if(channel->GetPlaybackChannel()->InitThread())
			    //    {
				   //     std::cout << "Error on Playback InitThread A" << std::endl;
				   //     system("pause");
				   //     return 0;
			    //    }
       //         }
			    //playback_counter++;
       //     }
		}			
	}
    //return true;

    SetVideoOutput( true );

    return true;
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
        auto captureChannel = channel->GetCaptureChannel();
        if( playbackChannel && 
            ( !captureChannel /*|| ( captureChannel && m_playthrough==false )*/ ) )
		{
            playbackChannel->m_pFifoBuffer->PutLiveBuffer( new CFrame( reinterpret_cast< const unsigned char * >( data->Get() ), 1, playbackChannel->GoldenSize, playbackChannel->BytesPerLine ) );
		}
	}   
}

//**************************************
//
VideoCard::ChannelOptionMap     VideoCard::CreateChannelOptionMap   ()
{
    ChannelOptionMap channelOptionMap;

    ChannelOption A;
    A.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_A;
    A.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_A;
    A.EpochSDIInput = EPOCH_SRC_SDI_INPUT_A;
    A.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_A;
    A.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHA;
    A.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA;

    ChannelOption B;
    B.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_B;
    B.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_B;
    B.EpochSDIInput = EPOCH_SRC_SDI_INPUT_B;
    B.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_B;
    B.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHB;
    B.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHB;

    ChannelOption C;
    C.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_C;
    C.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_C;
    C.EpochSDIInput = EPOCH_SRC_SDI_INPUT_C;
    C.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_C;
    C.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHC;
    C.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHC;

    ChannelOption D;
    D.InputChannel = BLUE_VIDEO_INPUT_CHANNEL_D;
    D.OutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_D;
    D.EpochSDIInput = EPOCH_SRC_SDI_INPUT_D;
    D.EpochSDIOutput = EPOCH_DEST_SDI_OUTPUT_D;
    D.EpochInputMemInterface = EPOCH_DEST_INPUT_MEM_INTERFACE_CHD;
    D.EpochOutputMemInterface = EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHD;

    channelOptionMap[ ChannelName::A ] = A;
    channelOptionMap[ ChannelName::B ] = B;
    channelOptionMap[ ChannelName::C ] = C;
    channelOptionMap[ ChannelName::D ] = D;

    return channelOptionMap;
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
//
////**************************************
////
//BOOL BlueFishVideoCard::IsInputChannel(EBlueVideoChannel VideoChannel)
//{
//    if(VideoChannel == BLUE_VIDEO_INPUT_CHANNEL_A ||
//        VideoChannel == BLUE_VIDEO_INPUT_CHANNEL_B ||
//        VideoChannel == BLUE_VIDEO_INPUT_CHANNEL_C ||
//        VideoChannel == BLUE_VIDEO_INPUT_CHANNEL_D)
//        return TRUE;
//
//    return FALSE;
//}
//
////**************************************
////
//BOOL BlueFishVideoCard::IsOutputChannel(EBlueVideoChannel VideoChannel)
//{
//    if(VideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_A ||
//        VideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_B ||
//        VideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_C ||
//        VideoChannel == BLUE_VIDEO_OUTPUT_CHANNEL_D)
//        return TRUE;
//
//    return FALSE;
//}
//
////**************************************
////
//void BlueFishVideoCard::InitBluefish(CBlueVelvet4* tempSDK, int CardId, EBlueVideoChannel VideoChannel, EVideoMode VideoMode, EUpdateMethod UpdFmt, EMemoryFormat MemoryFormat, EEngineMode VideoEngine)
//{
//	{CardId;}
//    VARIANT varVal;
//    /*CBlueVelvet4* tempSDK=NULL;
//    tempSDK = BlueVelvetFactory4();
//    tempSDK->device_enumerate(iDevices);
//
//    if(iDevices > 0 && CardId <= iDevices)
//        tempSDK->device_attach(CardId, 0);
//    else
//    {
//        BlueVelvetDestroy(tempSDK);
//        return NULL;
//    }*/
//    varVal.vt = VT_UI4;
//    if(IsOutputChannel(VideoChannel))
//    {
//        varVal.ulVal = m_referenceMode;
//        tempSDK->SetCardProperty(VIDEO_GENLOCK_SIGNAL, varVal);
//
//        unsigned int HPhase=m_refH, VPhase=m_refV; //random value for test purposes
//        varVal.ulVal = HPhase;
//        varVal.ulVal |= ((VPhase & 0xFFFF) << 16);
//        tempSDK->SetCardProperty(GENLOCK_TIMING, varVal);
//        //NOTE: when using BlueSoftware mode the accepted
//
//
//        varVal.ulVal = VideoChannel;
//        tempSDK->SetCardProperty(DEFAULT_VIDEO_OUTPUT_CHANNEL, varVal);
//
//        tempSDK->video_playback_stop(0, 0);
//
//        varVal.ulVal = VideoMode;
//        tempSDK->SetCardProperty(VIDEO_MODE, varVal);
//        tempSDK->QueryCardProperty(VIDEO_MODE, varVal);
//
//        varVal.ulVal = MemoryFormat;
//        tempSDK->SetCardProperty(VIDEO_MEMORY_FORMAT, varVal);
//
//        varVal.ulVal = UpdFmt;
//        tempSDK->SetCardProperty(VIDEO_UPDATE_TYPE, varVal);
//
//        
//        PixelsPerLine = BlueVelvetLinePixels(VideoMode);
//        VideoLines = BlueVelvetFrameLines(VideoMode, UpdFmt);
//
//        varVal.ulVal = ENUM_BLACKGENERATOR_OFF;
//        tempSDK->SetCardProperty(VIDEO_BLACKGENERATOR, varVal);
//
//        varVal.ulVal = VideoEngine;
//        tempSDK->SetCardProperty(VIDEO_OUTPUT_ENGINE, varVal);
//
//
//		if(outputsManager->Outputs.size()==2 && outputsManager->Outputs[0]->m_outputConfig->type=="FILL" && outputsManager->Outputs[1]->m_outputConfig->type=="KEY")
//		{
//			RouteChannel(pSDK, EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_SDI_OUTPUT_A, BLUE_CONNECTOR_PROP_DUALLINK_LINK_1);
//			RouteChannel(pSDK, EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_SDI_OUTPUT_B, BLUE_CONNECTOR_PROP_DUALLINK_LINK_2);
//		}
//		else if(outputsManager->Outputs.size()==2 && outputsManager->Outputs[1]->m_outputConfig->type=="FILL" && outputsManager->Outputs[0]->m_outputConfig->type=="KEY")
//		{
//			RouteChannel(pSDK, EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_SDI_OUTPUT_A, BLUE_CONNECTOR_PROP_DUALLINK_LINK_2);
//			RouteChannel(pSDK, EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_SDI_OUTPUT_B, BLUE_CONNECTOR_PROP_DUALLINK_LINK_1);
//		}
//		else if(outputsManager->Outputs.size()==1 && outputsManager->Outputs[0]->m_outputConfig->type=="FILL")
//		{
//			RouteChannel(pSDK, EPOCH_SRC_OUTPUT_MEM_INTERFACE_CHA, EPOCH_DEST_SDI_OUTPUT_A, BLUE_CONNECTOR_PROP_SINGLE_LINK);
//		}
//		else
//			cout << "UNKNOWN CHANNEL CONFIGURATION" << endl;
//
//        //varVal.ulVal = 1; //turn dual link on
//        //tempSDK->SetCardProperty(VIDEO_DUAL_LINK_OUTPUT, varVal); //sets routing automatically
//
//        //varVal.ulVal = Signal_FormatType_4224;
//        //tempSDK->SetCardProperty(VIDEO_DUAL_LINK_OUTPUT_SIGNAL_FORMAT_TYPE, varVal);
//
//        //ToDo: Orientation
//        varVal.ulVal = ImageOrientation_VerticalFlip;
//        tempSDK->SetCardProperty(VIDEO_IMAGE_ORIENTATION, varVal);
//
//        FieldCount = 0;
//        for(int i=0; i<10; i++)
//        {
//            tempSDK->wait_output_video_synch(UpdateFormat, FieldCount);
//        }
//
//    }
//    else
//    {
//        cout << "Wrong channel: " << VideoChannel << endl;
//        tempSDK->device_detach();
//        BlueVelvetDestroy(tempSDK);
//    }
//
//    //return tempSDK;
//}
//
////**************************************
////
//bool BlueFishVideoCard::DirectGPUPreRender()
//{
//   
//    return true;
//}
//
////**************************************
////
//bool BlueFishVideoCard::DirectGPUPostRender()
//{
//   
//    return true;
//}
//
////**************************************
////
//void BlueFishVideoCard::AdjustProcessWorkingSet()
//{
//    // Increase the process working set size to allow pinning of more memory.
//    static SIZE_T  dwMin = 0, dwMax = 0;
//    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA, FALSE, GetCurrentProcessId());
//
//    if(!hProcess)
//    {
//        printf( "OpenProcess failed (%d)\n", GetLastError() );
//    }
//
//    // Retrieve the working set size of the process.
//    if (!dwMin && !GetProcessWorkingSetSize(hProcess, &dwMin, &dwMax))
//    {
//        printf("GetProcessWorkingSetSize failed (%d)\n",
//            GetLastError());
//    }
//
//    int size = 1920*1080*4;
//    BOOL res = SetProcessWorkingSetSize(hProcess, size*NUMBER_OGL_TEXTURES*4 + dwMin, size*NUMBER_OGL_TEXTURES*4 + (dwMax-dwMin));
//    if(!res)
//        printf("SetProcessWorkingSetSize failed (%d)\n", GetLastError());
//
//    CloseHandle(hProcess);
//}
//
////**************************************
////
//int BlueFishVideoCard::InitSDKGPUDirect( const std::vector<int> & hackBuffersUids )
//{
//    EBlueVideoChannel nVideoOutputChannel;
//
//    //VideoMode = ParseVideoMode(outputsManager->Outputs[0]->m_outputConfig);
//
//    //EVideoMode VideoMode = VID_FMT_PAL;
//    //EVideoMode VideoMode = VID_FMT_NTSC;
//    EUpdateMethod UpdateFormat = UPD_FMT_FRAME;
//    //EUpdateMethod UpdateFormat = UPD_FMT_FIELD;
//    EMemoryFormat MemoryFormat = MEM_FMT_BGRA;
////    int MemFmtSelection = 0;
//
//    nVideoOutputChannel = BLUE_VIDEO_OUTPUT_CHANNEL_A;
//
//    MemoryFormat = MEM_FMT_BGRA;
//
//    //pSDK = NULL;
//    InitBluefish(pSDK, this->device_id, nVideoOutputChannel, (EVideoMode)VideoMode, UpdateFormat, MemoryFormat, VIDEO_ENGINE_PLAYBACK);
//    if(!pSDK)
//    {
//        cout << "Error initialising Bluefish card" << endl;
//        return 0;
//    }
//    
//    AdjustProcessWorkingSet();
//
//    unsigned int nCardType = (unsigned int)pSDK->has_video_cardtype();
//
//    GLenum tmp = BVGL::bvglGetError();
//    if(GL_NO_ERROR != tmp)
//    {
//        cout << "GL Error in FifoThread start: " << tmp << endl;
//        return FALSE;
//    }
//
//    GLenum glType = GL_RGBA;
//
//    switch(MemoryFormat)
//    {
//    case MEM_FMT_BGRA:
//    case MEM_FMT_RGBA:
//        glType = GL_RGBA8;
//        break;
//    case MEM_FMT_BGR:
//        glType = GL_RGB8;
//        break;
//    case MEM_FMT_BGR_16_16_16:
//        glType = GL_RGB16;
//        break;
//    case MEM_FMT_BGRA_16_16_16_16:
//        glType = GL_RGBA16;
//        break;
//    case MEM_FMT_CINEON_LITTLE_ENDIAN:
//        glType = GL_RGB10;
//        break;
//    case MEM_FMT_YUVS:
//    case MEM_FMT_2VUY:
//        glType = GL_RGBA8;
//        break;
//    default:
//        cout << "Not supported pixel format!" << endl;
//        return 0;
//    }
//    glOutputBuffersHack.push_back(0);
//    glOutputBuffersHack.push_back(hackBuffersUids[0]);
//    glOutputBuffersHack.push_back(hackBuffersUids[1]);
//    pGpuDirectOut = bfGpuDirect_Init( 1, nVideoOutputChannel, NULL, GPUDIRECT_CONTEXT_TYPE_OPENGL, &glOutputBuffersHack[0], (unsigned int)glOutputBuffersHack.size(), NUMBER_CHUNKS);
//
//    if(!pGpuDirectOut)
//    {
//        cout << "Error initialising BlueGpuDirect" << endl;
//        return 0;
//    }
//    int GpuDirectError = BGD_NO_ERROR;
//    int GpuDirectLowLevelError = 0;
//    GpuDirectError = bfGpuDirect_GetLastError(pGpuDirectOut, &GpuDirectLowLevelError);
//    if(GpuDirectError != BGD_NO_ERROR)
//    {
//        cout << "Error initialising BlueGpuDirect: " << GpuDirectError << ", LowLevelError: " << GpuDirectLowLevelError << endl;
//        return 0;
//    }
//    
//     cout << BlueVelvetVersion() << endl;;
//
//    FieldCount = 0;
//    BufferSelect = 0;
//    TextureSelect = 0;
//    DMABufferIdVideo = 0;
//    DMABufferIdHanc = 0;
//    DMABufferIdVanc = 0;
//    nOglTexID = 0;
//    nVancBufferSize = 0;
//    bUseAudio = FALSE;
//    bUseVanc = FALSE;
//
//    if(bUseAudio)
//    {
//        g_pAudioScratch = new unsigned int[2002*16];
//        Fill50((unsigned short*)g_pAudioScratch, 1600, 2);
//        g_pHancFrame = (BLUE_UINT8*)VirtualAlloc(NULL, 256*1024, MEM_COMMIT, PAGE_READWRITE);
//        VirtualLock(g_pHancFrame, 256*1024);
//    }
//
//    if(bUseVanc)
//    {
//        nVancBufferSize = BlueVelvetVANCGoldenValue(nCardType, VideoMode, MEM_FMT_BGRA, (UpdateFormat == UPD_FMT_FRAME)?BLUE_DATA_FRAME:BLUE_DATA_FIELD1);
//        g_pVancData = (BLUE_UINT8*)VirtualAlloc(NULL, nVancBufferSize, MEM_COMMIT, PAGE_READWRITE);
//        VirtualLock(g_pVancData, nVancBufferSize);
//
//        unsigned int PixelsPerLine = BlueVelvetLinePixels(VideoMode);
//        unsigned int VbiVancLineCount = BlueVelvetFrameLines(VideoMode, UpdateFormat);
//        blue_init_vanc_buffer(nCardType, VideoMode, PixelsPerLine, VbiVancLineCount, (UINT32*)g_pVancData);
//    }
//    nCardType = (unsigned int)pSDK->has_video_cardtype();
//    iFramesToBuffer = 2;
//    pSDK->wait_output_video_synch(UpdateFormat, FieldCount);
//	if(UpdateFormat == UPD_FMT_FIELD && !(FieldCount & 0x1))
//		pSDK->wait_output_video_synch(UpdateFormat, FieldCount);	//start on correct field
//    return true;
//}

////**************************************
////
//void BlueFishVideoCard::InitBuffer(BLUE_UINT8* pVideoBuffer, ULONG PixelsPerLine, ULONG VideoLines)
//{
//    //For the test purpose of this sample we simply fill the buffer with a solid color
//    BLUE_UINT8* pTmp = (BLUE_UINT8*)pVideoBuffer;
//
//    for(ULONG i=0; i<PixelsPerLine*VideoLines/3; i++)
//    {
//        //BLUE
//        *pTmp = 0x00; pTmp++;
//        *pTmp = 0x00; pTmp++;
//        *pTmp = 0x00; pTmp++;
//    }
//
//    for(ULONG i=0; i<PixelsPerLine*VideoLines/3; i++)
//    {
//        //GREEN
//        *pTmp = 0x00; pTmp++;
//        *pTmp = 0x00; pTmp++;
//        *pTmp = 0x00; pTmp++;
//    }
//
//    for(ULONG i=0; i<PixelsPerLine*VideoLines/3; i++)
//    {
//        //RED
//        *pTmp = 0x00; pTmp++;
//        *pTmp = 0x00; pTmp++;
//        *pTmp = 0x00; pTmp++;
//    }
//}

////**************************************
////
//void BlueFishVideoCard::SetReferenceModeValue(std::string refMode)
//{
//    if(refMode=="FREERUN") m_referenceMode=BlueFreeRunning;
//    else if(refMode=="IN_A") m_referenceMode=BlueSDI_A_BNC;
//    else if(refMode=="IN_B") m_referenceMode=BlueSDI_B_BNC;
//    else if(refMode=="ANALOG") m_referenceMode=BlueAnalog_BNC;
//    else if(refMode=="GENLOCK") m_referenceMode=BlueGenlockBNC;
//    else 
//    {
//        std::cout << "CONFIG REFERENCE MODE NOT SUPPORTED. USING FREERUN" << std::endl;
//        m_referenceMode=BlueFreeRunning;
//    }
//}

//**************************************
//
bool	VideoCard::CheckIfNewFrameArrived( ChannelName channelName )
{
    auto channel = GetChannelByName( channelName );
    if( channel && channel->GetCaptureBuffer() )
    {
        return !channel->GetCaptureBuffer()->IsLiveBufferEmpty();
    }
    return false;
}
//**************************************
//
unsigned char *	VideoCard::GetCaptureBufferForShaderProccessing( ChannelName channelName )
{
    auto channel = GetChannelByName( channelName );
    if( channel && channel->GetCaptureChannel() )
    {
        return channel->GetCaptureChannel()->m_pFifoBuffer->GetLiveBuffer()->m_pBuffer;
    }
    return nullptr;
}

////**************************************
////
//void	BlueFishVideoCard::UnblockCaptureQueue( ChannelName channelName )
//{
//    return GetChannelByName( channelName )->GetCaptureBuffer()->m_threadsafebuffer.unblockPush();
//}
//
////**************************************
////
//bool VideoCard::UpdateReferenceOffset(std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV)
//{
//	return GetChannelByName(ChannelName)->GetPlaybackChannel()->UpdateReferenceOffset(refH, refV);
//}
//
////**************************************
////
//bool VideoCard::UpdateReferenceMode(std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/)
//{
//	long referenceMode = 0;
//
//	if(ReferenceModeName=="FREERUN") referenceMode = BlueFreeRunning;
//    else if(ReferenceModeName=="IN_A") referenceMode = BlueSDI_A_BNC;
//    else if(ReferenceModeName=="IN_B") referenceMode = BlueSDI_B_BNC;
//    else if(ReferenceModeName=="ANALOG") referenceMode = BlueAnalog_BNC;
//    else if(ReferenceModeName=="GENLOCK") referenceMode = BlueGenlockBNC;
//    else 
//    {
//        std::cout << "CONFIG REFERENCE MODE NOT SUPPORTED. USING FREERUN" << std::endl;
//        referenceMode = BlueFreeRunning;
//    }
//
//	return GetChannelByName(ChannelName)->GetPlaybackChannel()->UpdateReferenceMode(referenceMode);
//}

////**************************************
////
//UInt32  BlueFishVideoCard::DetectInputs         ()
//{
//    VARIANT varVal;
//    varVal.ulVal = 0;
//    if( BLUE_FAIL( m_SDK->QueryCardProperty( CARD_FEATURE_STREAM_INFO, varVal ) ) )
//    {
//        std::cout << "Function not supported; need driver 5.10.2.x" << std::endl;
//        system("pause");
//        BailOut(pSDK);
//        return 0;
//    }
//
//    return CARD_FEATURE_GET_SDI_INPUT_STREAM_COUNT(varVal.ulVal);
//}
//
////**************************************
////
//UInt32  BlueFishVideoCard::DetectOutputs        ()
//{    
//    VARIANT varVal;
//    varVal.ulVal = 0;
//    if(BLUE_FAIL( m_SDK->QueryCardProperty(CARD_FEATURE_STREAM_INFO, varVal)))
//    {
//        std::cout << "Function not supported; need driver 5.10.2.x" << std::endl;
//        system("pause");
//        BailOut(pSDK);
//        return 0;
//    }
//
//    return CARD_FEATURE_GET_SDI_OUTPUT_STREAM_COUNT(varVal.ulVal);
//}

} //bluefish
} //videovcards
} //bv
