#include "BlackMagicVideoCard.h"

#include "Serialization/SerializationHelper.h"


#define SUCCESS( hr ) ( ( ( HRESULT )( hr ) ) == S_OK )


namespace bv { namespace videocards { namespace blackmagic {


//**************************************
//
VideoCardDesc::VideoCardDesc()
    : m_uid( "BlackMagic" )
{
}

//**************************************
//
IVideoCardPtr           VideoCardDesc::CreateVideoCard( const IDeserializer & deser ) const
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

        if( deser.EnterChild( "channels" ) )
        {
            if( deser.EnterChild( "channel" ) )
            {
                do
                {
                    if( deser.EnterChild( "output" ) )
                    {
                        ChannelOutputData output;
                        output.type = SerializationHelper::String2T< IOType >( deser.GetAttribute( "type" ) );
                        output.resolution = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "resolution" ), 1080 );
                        output.refresh = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( "refresh" ), 5000 );
                        output.interlaced = SerializationHelper::String2T< bool >( deser.GetAttribute( "interlaced" ), false );
                        output.flipped = SerializationHelper::String2T< bool >( deser.GetAttribute( "flipped" ), false );
                        output.videoMode = ConvertVideoMode( output.resolution, output.refresh, output.interlaced );

                        deser.ExitChild(); //output

                        card->AddOutput( output );
                    }
                }
                while( deser.NextChild() );

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
const std::string &     VideoCardDesc::GetVideoCardUID  () const
{
    return m_uid;
}


//**************************************
//
VideoCard::VideoCard( UInt32 deviceID )
    : m_deviceID( deviceID )
    , m_device( nullptr )
    , m_output( nullptr )
    , m_configuration( nullptr )
{
    InitVideoCard();
}

//**************************************
//
VideoCard::~VideoCard       ()
{
    while( !m_frames.empty() )
    {
        m_frames[ 0 ]->Release();
    }
    m_frames.clear();

    if( m_output )
        m_output->Release();

    if( m_configuration )
        m_configuration->Release();

    if( m_device )
        m_device->Release();
}

//**************************************
//
bool                    VideoCard::InitVideoCard        ()
{
    if( InitDevice() )
    {
        return InitOutput();
    }

    return false;
}

//**************************************
//
bool                    VideoCard::InitDevice           ()
{
    IDeckLinkIterator * iterator;

    if( SUCCESS( CoCreateInstance( CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, ( void** )&iterator ) ) )
    {
        auto idx = m_deviceID;
        while( SUCCESS( iterator->Next( &m_device ) ) )
        {
            if( idx == 0 )
            {
                break;
            }
            --idx;

            m_device->Release();
        }

        if( m_device )
        {
            if( SUCCESS( m_device->QueryInterface( IID_IDeckLinkOutput, ( void** )&m_output ) ) &&
                SUCCESS( m_device->QueryInterface( IID_IDeckLinkConfiguration, ( void** )&m_configuration ) ) )
            {
                return true;
            }
        }

        iterator->Release();
    }

    return false;
}

//**************************************
//
bool                    VideoCard::InitOutput()
{
    IDeckLinkDisplayModeIterator * displayModeIterator = nullptr;
    IDeckLinkDisplayMode * displayMode = nullptr;

    for( auto & output : m_outputs )
    {
        if( SUCCESS( m_output->GetDisplayModeIterator( &displayModeIterator ) ) )
        {
            while( SUCCESS( displayModeIterator->Next( &displayMode ) ) )
            {
                if( displayMode->GetDisplayMode() == output.videoMode )
                {
                    break;
                }

                displayMode->Release();
            }

            displayModeIterator->Release();

            if( displayMode )
            {
                auto width = displayMode->GetWidth();
                auto height = displayMode->GetHeight();

                IDeckLinkMutableVideoFrame * frame = nullptr;

                if( SUCCESS( m_output->EnableVideoOutput( displayMode->GetDisplayMode(),
                                                            BMDVideoOutputFlags::bmdVideoOutputFlagDefault ) ) &&
                    SUCCESS( m_output->CreateVideoFrame( width, height, width * 4, BMDPixelFormat::bmdFormat8BitBGRA,
                                                            bmdFrameFlagFlipVertical, &frame ) ) )
                {
                    m_frames.push_back( frame );
                    return true;
                }

                displayMode->Release();
            }

            displayModeIterator->Release();
        }
    }
    
    return false;
}

//**************************************
//
void                    VideoCard::SetVideoOutput       ( bool enable )
{
    { enable; }
    //for( auto frame : m_frames )
    //{
    //    void * rawFrame;
    //    frame->GetBytes( &rawFrame );

    //    memset( rawFrame, 0, frame->GetRowBytes() * frame->GetHeight() );
    //}
}

//**************************************
//
void                    VideoCard::AddOutput            ( ChannelOutputData output )
{
    m_outputs.push_back( output );
}

//**************************************
//
void                    VideoCard::Start                ()
{
}

//**************************************
//
void                    VideoCard::ProcessFrame         (AVFramePtr src_frame, int odd )
{
	{odd;}
    for( UInt32 i = 0; i < ( UInt32 )m_outputs.size(); ++i )
    {
        auto frame = m_frames[ i ];

        void * rawFrame;
        frame->GetBytes( &rawFrame );

        if( m_outputs[ i ].type == IOType::KEY )
        {
            //FIXME: CopyAlphaBits
        }
        else
        {
            memcpy( rawFrame, src_frame->m_videoData->Get(), frame->GetRowBytes() * frame->GetHeight() );
        }

        m_output->DisplayVideoFrameSync( frame );
    }
}

//**************************************
//
UInt32                          VideoCard::AvailableVideoCards = EnumerateDevices();

//**************************************
//
UInt32                  VideoCard::EnumerateDevices     ()
{
    Int32 deviceCount = 0;

    IDeckLink * deckLink = nullptr;
    IDeckLinkIterator * iterator = nullptr;
    if( SUCCESS( CoInitialize( nullptr ) ) )
    {
        if( SUCCESS( CoCreateInstance( CLSID_CDeckLinkIterator, nullptr, CLSCTX_ALL, IID_IDeckLinkIterator, ( void ** )&iterator ) ) )
        {
            while( SUCCESS( iterator->Next( &deckLink ) ) )
            {
                deviceCount++;
            }

            iterator->Release();
            deckLink->Release();
        }
    }

    return ( UInt32 )deviceCount;
}

} //blackmagic
} //videocards
} //bv

////**************************************
////
//void CopyAlphaBits(void* from, void *to,int size)
//{
//    __m128i mask;
//	mask.m128i_u8[0]  = 0x03;
//	mask.m128i_u8[1]  = 0x03;
//	mask.m128i_u8[2]  = 0x03;
//	mask.m128i_u8[3]  = 0x03;
//	mask.m128i_u8[4]  = 0x07;
//	mask.m128i_u8[5]  = 0x07;
//	mask.m128i_u8[6]  = 0x07;
//	mask.m128i_u8[7]  = 0x07;
//	mask.m128i_u8[8]  = 0x0b;
//	mask.m128i_u8[9]  = 0x0b;
//	mask.m128i_u8[10] = 0x0b;
//	mask.m128i_u8[11] = 0x0b;
//	mask.m128i_u8[12] = 0x0f;
//	mask.m128i_u8[13] = 0x0f;
//	mask.m128i_u8[14] = 0x0f;
//	mask.m128i_u8[15] = 0x0f;
//
//	__m128i * in_128  = (__m128i*) from;
//	__m128i * out_128 = (__m128i*) to;
//
//	int steps = size / (4*4);
//
//	for (int i = 0;i < steps; ++i, ++in_128, ++out_128 )
//	{
//		_mm_store_si128( out_128, _mm_shuffle_epi8( _mm_load_si128( in_128 ), mask ) );	
//	}
//
//}
//
////**************************************
////
//void BlackMagicVideoCard::Black()
//{
//   unsigned char *buffer = new unsigned char[2048*2048*4];
//   DeliverFrameFromRAM(buffer);
//}
//
////**************************************
////
//int BlackMagicVideoCard::getChannelsCount()
//{
//
//    return ChannelsCount;
//}
//
////**************************************
////
//bool BlackMagicVideoCard::InitOutputChannels()
//{
//    VideoOutput *defaultOutput = outputsManager->CreateVideoOutput();
//    bool result=false;
//    if(SuperMagic)
//    {
//        defaultOutput->SetVideoOutputType(VideoOutputType::FILL_KEY);
//        result = defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,1,VideoOutputType::FILL_AUTO_KEY));
//    }else{
//        defaultOutput->SetVideoOutputType(VideoOutputType::FILL_KEY);
//        result = defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,1,VideoOutputType::FILL));
//        result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,2,VideoOutputType::KEY));
//    }
//    
//    /*
//    result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,3,VideoOutputType::FILL));
//    result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,4,VideoOutputType::FILL));
//    result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,5,VideoOutputType::FILL));*/
//    //result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,6,VideoOutputType::FILL));
//    /*result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,3,VideoOutputType::FILL));
//    result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,4,VideoOutputType::FILL));
//    result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,5,VideoOutputType::FILL));
//    result = result && defaultOutput->AddChannel(VideoOutputDescriptor(VideoCardMode::HD_50_I,6,VideoOutputType::FILL));*/
//
//    
//    return result;
//}
//
////**************************************
////
//bool BlackMagicVideoCard::InitVideoOuput(IDeckLinkOutput *output,VideoOutputDescriptor videoMode)
//{
//    IDeckLinkDisplayModeIterator*		        pDLDisplayModeIterator;
//	IDeckLinkDisplayMode*				        pDLDisplayMode = NULL;
//    BMDDisplayMode                              DisplayModes[6];
//
//    DisplayModes[VideoCardMode::HD_50_I]    =   BMDDisplayMode::bmdModeHD1080i50;
//    DisplayModes[VideoCardMode::HD_25_P]    =   BMDDisplayMode::bmdModeHD1080p25;
//    DisplayModes[VideoCardMode::SD_50_I]    =   BMDDisplayMode::bmdModePAL;
//    DisplayModes[VideoCardMode::SD_25_P]    =   BMDDisplayMode::bmdModePALp;
//
//    IDeckLinkMutableVideoFrame*                 VideoFrame;
//
//
//    bool                                        displayModeFound = false;
//
//    if (output->GetDisplayModeIterator(&pDLDisplayModeIterator) == S_OK)
//	{
//		while (pDLDisplayModeIterator->Next(&pDLDisplayMode) == S_OK)
//		{
//            if(pDLDisplayMode->GetDisplayMode() == DisplayModes[videoMode.displayMode])
//            {
//                displayModeFound = true;
//
//                if (output->EnableVideoOutput(pDLDisplayMode->GetDisplayMode(), bmdVideoOutputFlagDefault) != S_OK)
//                {
//
//                    //printf("VideoCard","ERROR","Failed initialising Deckling video output...");
//                    printf("VideoCard ERROR - failed initialising Deckling video output...\n");
//                    return false;
//                }
//
//              
//
//                int VideoId=-1;
//                for(int i=0;i<ChannelsCount;i++)
//                {
//                    if(HardwareOutputs[i]==output)
//                    {
//                        VideoId         = i;
//                        break;
//                    }
//                }
//                if(VideoId==-1)
//                {
//                    printf("VideoCard ERROR VideoId =-1 :(\n");
//                    return false;
//                }
//                uiTotalFrames[VideoId]=0;
//                uiFrameWidth[VideoId] = pDLDisplayMode->GetWidth();
//	            uiFrameHeight[VideoId] = pDLDisplayMode->GetHeight();
//                pDLDisplayMode->GetFrameRate(&frameDuration[VideoId], &frameTimescale[VideoId]);
//                if (output->CreateVideoFrame(uiFrameWidth[VideoId], uiFrameHeight[VideoId], uiFrameWidth[VideoId]*4, bmdFormat8BitBGRA, bmdFrameFlagFlipVertical, &VideoFrame) != S_OK)
//		            {
//
//                         printf("VideoCard - ERROR - Failed creating video frame...\n");
//                        return false;
//                    }
//                
//                VideoFrames[VideoId]  =   VideoFrame;
//
//                
//	            VideoFrame->GetBytes((void**)&pFrame[VideoId]);
//                int size = VideoFrame->GetRowBytes() * uiFrameHeight[VideoId];
//
//	            memset(pFrame[VideoId], 0xA3, size);
//
//                //RenderDelegates[VideoId] = new RenderDelegate(this,VideoId);
//
//                /*if (output->SetScheduledFrameCompletionCallback( RenderDelegates[VideoId]) != S_OK)
//                    {
//
//                        printf("VideoCard","ERROR","Setting delegate...");
//                        return false;
//                    }*/
//              
//
//                /*for (unsigned __int32 i=0; i < 5; i++)
//	            {
//		            if (output->ScheduleVideoFrame(VideoFrames[VideoId] , (uiTotalFrames[VideoId] * frameDuration[VideoId]), frameDuration[VideoId], frameTimescale[VideoId]) != S_OK)
//			            return false;
//		            uiTotalFrames[VideoId]++;
//	            }*/
//
//                
//                output->StartScheduledPlayback(0, 100, 1.0);
//
//                return true;
//            }
//			
//		}
//		
//	}
//
//    pDLDisplayModeIterator->Release();
//	return false;
//
//
//
//}
//
//
////**************************************
////
//bool BlackMagicVideoCard::InitHardwareOutputs()
//{
//
//    VideoOutput* output = outputsManager->GetDefaultVideoOutput();
//    
//    for(unsigned int i=0;i<output->Channels.size();i++)
//    {
//        int hdVideo =  output->Channels[i].OutputId-1;
//        printf("VideoCard INFO Trying to initialize ");
//		printf(to_string(i).c_str());
//        printf(" device : id(");
//		printf(to_string(hdVideo).c_str());
//        printf(") \n");
//        
//        if(hdVideo<0 || hdVideo >= ChannelsCount)
//        {
//            printf("VideoCard ERROR InitHardwareOutputs(), wrong Output id!");
//            return false;
//        }
//        if(this->InitVideoOuput(HardwareOutputs[hdVideo],output->Channels[i]))
//        {
//            printf("VideoCard","INFO", "BlackMagic video output ["+to_string(hdVideo+1)+"] initialised! \n");
//        }else{
//            printf("VideoCard ERROR BlackMagic video output [");
//			printf(to_string(hdVideo+1).c_str());
//            printf("] initialisation failed! \n");
//            return false;
//        }
//    }
//
//    for(unsigned int i=0;i<output->Channels.size();i++)
//    {
//        int hdVideo =  output->Channels[i].OutputId-1;
//        if(hdVideo<0 || hdVideo >= ChannelsCount)
//        {
//            printf("VideoCard ERROR InitHardwareOutputs(), wrong Output id! \n");
//            return false;
//        }
//       // HardwareOutputs[hdVideo]->StartScheduledPlayback(0, 100, 1.0);
//    }
//    return true;
//}
//
////**************************************
////
//int BlackMagicVideoCard::InitSDK()
//{
//        printf("VideoCard INFO Searching for Decklink devices... \n");
//        int numDevices=0;
//		InitializeCriticalSection(&pMutex);
//	    //IDeckLinkMutableVideoFrame*	pDLVideoFrame = NULL;
//	    IDeckLink *deckLink=NULL;
//	    //pFrame = malloc(1280*720*4);
//
//
//	    
//        CoInitialize(NULL);
//        IDeckLinkAPIInformation*	deckLinkAPIInformation;
//	    HRESULT result;
//        IDeckLinkIterator* pDLIterator = NULL;	
//         printf("VideoCard INFO ............. \n");
//	    result = CoCreateInstance(CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, (void**)&pDLIterator);
//        if(result != S_OK)
//        {
//            printf("VideoCard ERROR Brak sterownik�w? \n");
//            return -1;
//        }
//        result = pDLIterator->QueryInterface(IID_IDeckLinkAPIInformation, (void**)&deckLinkAPIInformation);
//
//        if (result == S_OK)
//	    {
//		    deckLinkAPIInformation->Release();
//        }else{ 
//            printf("VideoCard INFO No Decklink API present... \n");
//            return 0;
//        }
//
//	    //bool bSuccess = FALSE;
//	    //IDeckLink* pDL = NULL;
//
//        while (pDLIterator->Next(&deckLink) == S_OK)
//	    {
//		    numDevices++;	
//	    }
//		
//        pDLIterator->Release();
//        result = CoCreateInstance(CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, (void**)&pDLIterator);
//
//        int i=0;
//        printf("VideoCard INFO Detected [ ");
//        printf((to_string(numDevices)).c_str());
//        printf(" ] Decklink outputs \n");
//        ChannelsCount = numDevices;
//
//        HardwareOutputs     =   new IDeckLinkOutput*[ChannelsCount];
//        deckLinkKeyers      =   new IDeckLinkKeyer*[ChannelsCount];
//        uiFrameWidth        =   new unsigned __int32[ChannelsCount];
//	    uiFrameHeight       =   new unsigned __int32[ChannelsCount];
//	    frameDuration       =   new BMDTimeValue[ChannelsCount];
//        uiTotalFrames       =   new unsigned __int32 [ChannelsCount];
//	    frameTimescale      =   new BMDTimeValue[ChannelsCount];
//        pFrame              =   new void*[ChannelsCount];
//        RenderDelegates     =   new RenderDelegate*[ChannelsCount];
//        VideoFrames         =   new IDeckLinkMutableVideoFrame*[ChannelsCount];
//        Mutex               =   new CRITICAL_SECTION[ChannelsCount];
//
//        while (pDLIterator->Next(&deckLink) == S_OK)
//	    {
//		    BSTR deviceNameBSTR = NULL;
//			deckLink->GetModelName(&deviceNameBSTR);
//			wprintf(deviceNameBSTR);
//			printf("\n");		
//            if (deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&pDLOutput) == S_OK)
//            {
//                HardwareOutputs[i] = pDLOutput;
//                pDLOutput = nullptr;
//            }
//            if(SuperMagic)
//            {
//                if (deckLink->QueryInterface(IID_IDeckLinkKeyer, (void**)&deckLinkKeyers[i]) != S_OK)
//			    {
//				   printf("VideoCard ERROR  Could not obtain keying handle \n");											
//			    }else{
//                    deckLinkKeyers[i]->Enable(true);
//                    deckLinkKeyers[i]->SetLevel(255);
//                }
//            }
//            i++;
//		  
//	    }
//
//
//        // buffers for frames
//        for(int i=0;i<ChannelsCount;i++)
//        {
//            InitializeCriticalSection(&Mutex[i]);
//            pFrame[i] = _aligned_malloc(1920*1080*4,128);
//        }
//        
//
//
//        outputsManager = new VideoOutputsManager(ChannelsCount);
//        if(this->InitOutputChannels())
//        {
//            printf("VideoCard INFO Output channels configured, initialising hardware... \n");
//        }else{
//            printf("VideoCard ERROR Failed configuring video channels ... \n");
//            return false;
//        }
//        if(this->InitHardwareOutputs())
//        {
//            printf("VideoCard INFO Hardware outputs up and running! \n");
//        }else{
//            printf("VideoCard ERROR Failed initialising hardware outputs! \n");
//        }
//
//        return true;
//}
//
////**************************************
////
//void BlackMagicVideoCard::InitOutputChannel()
//{
//	
//
//
//}
//
//
//
////**************************************
////
//void BlackMagicVideoCard::InitBuffer()
//{
//	
//}
//}
//}