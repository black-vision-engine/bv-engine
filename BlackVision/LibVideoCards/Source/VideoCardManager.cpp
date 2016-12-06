#include "VideoCardManager.h"
#include <memory>
#include <iostream>
#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"

//#include "UseLoggerVideoModule.h"


namespace bv { namespace videocards {


//**************************************
//
std::vector< IVideoCardDesc * >  DefaultVideoCardDescriptors  ()
{
    std::vector< IVideoCardDesc * > descriptors;
    
    descriptors.push_back( new bluefish::VideoCardDesc() );
    descriptors.push_back( new blackmagic::VideoCardDesc() );

    return descriptors;
}

//**************************************
//
VideoCardProcessingThread::VideoCardProcessingThread    ()
    : m_running( true )
{
}

//**************************************
//
VideoCardProcessingThread::~VideoCardProcessingThread   ()
{
}

//**************************************
//
void            VideoCardProcessingThread::Run          ()
{
    while( m_running )
    {
        if( !VideoCardManager::Instance().ProcessFrame() )
        {
            break;
        }
    }
}



//**************************************
//
BVVideoFramePtr          VideoCardManager::KILLER_FRAME = nullptr;

//**************************************
//
VideoCardManager::VideoCardManager      ()
    : m_keyActive( true )
    , m_interlaceEnabled( true )
    , m_dislpayMode( DisplayMode::HD )
	, m_currentFrameNumber(0)
    , m_processThread( std::unique_ptr< VideoCardProcessingThread >( new VideoCardProcessingThread() ) )
	
{
	int size = 1920 * 1080 * 4 / 2;
	char *mem = new char[size];
	m_currentFrameData = MemoryChunkPtr(new MemoryChunk((char*)mem, size));
		 
}

//**************************************
//
VideoCardManager::~VideoCardManager     ()
{
    // kill processing thread with killer frame
    QueueFrame( KILLER_FRAME );

    {
        std::unique_lock< std::mutex > lock( m_mutex );
        for( auto & videoCard : m_videoCards )
        {
            videoCard->SetVideoOutput( false );
        }
        m_videoCards.clear();
    }

    for( auto desc : m_descVec )
    {
        delete desc;
    }
}

// *********************************
//
void                        VideoCardManager::ReadConfig            ( const IDeserializer & deser )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    if( deser.EnterChild( "videocards" ) )
    {
        if( deser.EnterChild( "videocard" ) )
        {
            do
            {
                auto name = deser.GetAttribute( "name" );
                if( IsRegistered( name ) )
                {
                    auto videocard = m_descMap[ name ]->CreateVideoCard( deser );
                    if( videocard )
                    {
                        m_videoCards.push_back( videocard );
                    }
                }
            } while( deser.NextChild() );

            deser.ExitChild(); //videocard
        }

        deser.ExitChild(); //videocards
    }
}

// *********************************
//
void                        VideoCardManager::RegisterDescriptors   ( const std::vector< IVideoCardDesc * > & descriptors )
{
    for( auto desc : descriptors )
    {
        if( !IsRegistered( desc->GetVideoCardUID() ) )
        {
            m_descMap[ desc->GetVideoCardUID() ] = desc;
            m_descVec.push_back( desc );
        }
    }
}

// *********************************
//
bool                        VideoCardManager::IsRegistered          ( const std::string & uid ) const
{
    return ( m_descMap.find( uid ) != m_descMap.end() );
}

//**************************************
//
void VideoCardManager::SetVideoOutput   ( bool enable )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    for( auto & videoCard : m_videoCards )
    {
        videoCard->SetVideoOutput( enable );
    }
}

//**************************************
//
void                        VideoCardManager::SetKey                ( bool active )
{
    m_keyActive = active;
}

//**************************************
//
void                        VideoCardManager::Start                 ()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    
    m_processThread->Start();
    for( auto & videoCard : m_videoCards )
    {
		videoCard->Start();
    }
}

// *********************************
//
void                        VideoCardManager::QueueFrame            (BVVideoFramePtr data )
{
    m_frameBuffer.Push( data );
}

// *********************************
//
bool                        VideoCardManager::ProcessFrame          ()
{
    auto data = m_frameBuffer.Pop();

    if( data )
    {
		short  int odd = m_currentFrameNumber % 2;
		m_currentFrameNumber++;
        if( m_interlaceEnabled )
        {

            data = InterlacedFrame( data, odd  );
        }
		odd = m_currentFrameNumber % 2;
        std::unique_lock< std::mutex > lock( m_mutex );
        for( auto & videoCard : m_videoCards )
        {
            videoCard->ProcessFrame( data,odd );
        }

        return true;
    }

    return false;
}


// *********************************
//
BVVideoFramePtr         VideoCardManager::InterlacedFrame       (BVVideoFramePtr frame, int odd )
{
	// poni¿sza funkcja wycina z [data] co Nt¹ b¹dŸ co N+1¹ liniê (zamiast pe³nej ramki przekazujemy pó³pole, zamiast InterlacedFrame powinno byæ bardziej coœ w stylu ConvertProgressiveFrameToField
	
	const char *mem_src = frame->m_VideoData->Get();

	int pixel_depth = 4;  // pobraæ poni¿sze informacje (wdepth,  width, height z configa, albo niech tu nie przychodzi RawData tylko jakoœ to opakowane w klasê typu Frame
	int width = 1920;
	int height = 1080;
	int bytes_per_line = width * pixel_depth;
	int size = width * height * 2 + 2048; // z jakiegos powodu trzeba dodawaæ 2048 bajtów  poniewa¿ funkcja Bluefisha CalculateGoldenValue () zwraca tyle bajtów dla pó³pola HD, trzeab sprawdziæ jak to bedzie wygl¹daæ w SD

	char *mem_dst = new char[size];  // pewnie nie ma co tutaj tego za kazdym razem tworzyæ...


	for (int i = odd, j = 0;i < height;i += 2, j++)
	{
		memcpy(&mem_dst[j*(bytes_per_line)], &mem_src[i*(bytes_per_line)], bytes_per_line);
	}
	
	MemoryChunkConstPtr ptr = MemoryChunkConstPtr(new MemoryChunk((char*)mem_dst, size));  // ponownie - pewnie nie ma co tego tutaj tworzyæ za ka¿dym razem...
	
	frame->m_VideoData = ptr;

    return frame;
}

//**************************************
//
IVideoCardPtr   VideoCardManager::GetVideoCard        ( UInt32 idx )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    if( idx < m_videoCards.size() )
    {
        return m_videoCards[ idx ];
    }
    return nullptr;
}

// *********************************
//
VideoCardManager &      VideoCardManager::Instance             ()
{
    static VideoCardManager instance;
    return instance;
}
































////**************************************
////
//void VideoCardManager::SetupVideoChannels()
//{
//    unsigned int bln = 0;
//    for(unsigned int i = 0 ; i < m_VideoCards.size() ; i++)
//    {
//        if( m_VideoCards[i]->GetBrand() == "BlueFish" )
//        {
//            if( i > m_VideoCardConfig.blueFishCount) continue;
//            bln++;
//            if(m_VideoCards[i]->DetectOutputs() < m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount)
//            {
//                printf("VideoCards ERROR Too many Channels to configure");                
//                return;
//            }
//
//
//
//            for(unsigned int z = 0; z < m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount;z++)
//            {
//                ((BlueFishVideoCard*)m_VideoCards[i])->AddChannel(m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].name,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.type,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].renderer,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.resolution,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refresh,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.interlaced,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.flipped,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].playback,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].capture,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.playthrough,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.type,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.referenceMode,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refH,
//                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refV);
//            }
//        }
//        else if(m_VideoCards[i]->GetBrand() == "BlackMagic")
//        {
//            //m_VideoCards[i]->SetReferenceModeValue(m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.referenceMode);
//            //m_VideoCards[i]->SetReferenceOffsetValue(m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.refH,m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.refV);
//        }
//    }  
//}
////**************************************
////
//void VideoCardManager::ReadConfig()
//{
//    if( !m_VideoCardConfig.ReadbackFlag )
//    {
//        return;
//    }
//
//    if(m_VideoCardConfig.BlueFish)
//    {
//        RegisterBlueFishCards();
//    }
//
//    if(m_VideoCardConfig.BlackMagic && false)
//    {
//        RegisterBlackMagicCards();
//        //if(m_VideoCardConfig.superMagic)
//        //{
//        //    m_SuperMagic=true;
//        //}
//    }
//
//    if(m_VideoCards.size()<=0)
//        return;
//
//
//    if(m_VideoCardConfig.resolutionOld=="SD")
//    {
//        m_dislpayMode =  DisplayMode::SD;
//    }
//}


//
////**************************************
////
//void VideoCardManager::StopVideoCards()
//{
//    Disable();
//}

////**************************************
////
//void VideoCardManager::SuspendVideoCards()
//{
//    for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
//    {
//      m_VideoCards[i]->SuspendVideoCardProccessing();
//    }
//}
//
////**************************************
////
//void VideoCardManager::ResumeVideoCards()
//{
//    for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
//    {
//      m_VideoCards[i]->ResumeVideoCardProccessing();
//    }
//}
////**************************************
////
//unsigned int __stdcall VideoCardManager::copy_buffer_thread( void * args )
//{
//  VideoCardManager* pParams = (VideoCardManager*)args;
//    static const unsigned int frames_count = 2;
//    static const unsigned int fhd = 1920 * 1080 * 4;
//    static const unsigned int width_bytes = 1920 * 4;
//    static unsigned char buf[ fhd * frames_count ];
//    static unsigned int cur_buf = 0;
//    static int counter=0;
//
//    static double max_readback=0;
//    bv::HighResolutionTimer GTimer;
//    //unsigned char * FinalFrame = new unsigned char[fhd];
//
//    while( !pParams->m_midgardThreadStopping )
//    {
//        //double writeStart = GTimer.CurElapsed();
//
//      auto mid = pParams->GetMidgard();
//      auto & buf_ = mid->Buffer();
//      auto frame = buf_.pop();
//
//      assert( frame );
//
//      auto data = frame->GetData();
//
//      assert( data );
//
//      auto rawData = (unsigned char*)data->Get();
//
//        unsigned char *  frameBuf = rawData;
//
//      bool enable_interlace = false;
//      if(enable_interlace)
//      {
//           unsigned int next_buf = ( cur_buf + 1 ) % frames_count;
//
//          memcpy( &buf[ next_buf * fhd ], frameBuf, fhd );       
//          unsigned char * prevFrameBuf = &buf[ cur_buf * fhd ];
//
//          for( unsigned int i = 0;  i < 1080; i += 2 )
//          {
//              unsigned int cur_i = i + 1;
//              unsigned int prev_i = i + 1;
//
//              unsigned int cur_scanline = width_bytes * cur_i;
//              unsigned int prev_scanline = width_bytes * prev_i;
//
//              memcpy(&frameBuf[ cur_scanline ], &prevFrameBuf[ prev_scanline ], width_bytes );
//
//          }
//      }
//      if(!pParams->m_key_active)
//          {
//              for(int i=0;i<1920*1080*4;i+=4)
//              {
//                  /*[ +i+0]=0;
//                  frameBuf[ cur_scanline+i+1]=0;
//                  frameBuf[ cur_scanline+i+2]=0;*/
//                  frameBuf[ i+3 ]=0;
//              }
//          }
//       
//      pParams->DeliverFrameFromRAM( frameBuf ); 
//
//        cur_buf = ( cur_buf + 1 ) % frames_count; 
//    }
//  cout << "Midgard Thread stopped..." << endl;
//    _endthreadex(0);
//    return true;
//}

//
////**************************************
////
//void VideoCardManager::RegisterBlueFishCards()
//{
//    //CBlueVelvet4Ptr pSDK( BlueVelvetFactory4() );
//
//    int numCards=0;
//    CBlueVelvet4* BlueFishSDK = BlueVelvetFactory4();
//    BlueFishSDK->device_enumerate(numCards);
//
//    for(int i=1; i<=numCards ;i++)
//    {
//        m_VideoCards.push_back((VideoCardBase*) new BlueFishVideoCard(i));
//    }
//    BlueVelvetDestroy(BlueFishSDK);
//    printf("VideoCards INFO Registered BlueFish video cards: %d \n", numCards);
//}
////**************************************
////
//void VideoCardManager::RegisterBlackMagicCards()
//{
//    int numCards = 0;
//    //IDeckLink *deckLink=NULL;
//    //CoInitialize(NULL);
//    //IDeckLinkIterator* pDLIterator = NULL;    
//    //CoCreateInstance(CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, (void**)&pDLIterator);
//    //while (pDLIterator->Next(&deckLink) == S_OK)
//   // {
//        m_VideoCards.push_back((VideoCardBase*) new BlackMagicVideoCard(numCards+1));
//        numCards++; 
//    //}
//
//
//    //pDLIterator->Release();
//
//    printf("VideoCards INFO Registered Black Magic video cards: %d \n", numCards);
//}
////**************************************
////
//void VideoCardManager::RegisterVideoCards()
//{
//    RegisterBlueFishCards();   
//    RegisterBlackMagicCards();
//}

////**************************************
////
//void VideoCardManager::DetectVideoCards()
//{
//    int numCards = 0;
//    CBlueVelvet4* BlueFishSDK = BlueVelvetFactory4();
//    BlueFishSDK->device_enumerate(numCards);
//    printf("VideoCards INFO Detected BlueFish video cards: %d \n",numCards);
//    delete BlueFishSDK;
//    numCards = 0;
//
//    IDeckLink *deckLink=NULL;
//    CoInitialize(NULL);
//    IDeckLinkIterator* pDLIterator = NULL;    
//    CoCreateInstance(CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, (void**)&pDLIterator);
//    while (pDLIterator->Next(&deckLink) == S_OK)
//    {
//        numCards++;   
//    }
//    pDLIterator->Release();
//    delete deckLink;
//    printf("VideoCards INFO Detected BlueFish video cards: %d \n",numCards);
//}

////**************************************
////
//void VideoCardManager::PushDataFromRenderer     ( MemoryChunkConstPtr data )
//{
//    m_dataQueue.Push( data );
//}

////**************************************
////
//void VideoCardManager::EnableVideoCards()
//{
//    for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
//    {
//        m_VideoCards[i]->Enable();
//    }
//}
//
////**************************************
////
//void VideoCardManager::EnableVideoCard(int i)
//{
//    m_VideoCards[i]->Enable(); 
//}
//
////**************************************
////
//void VideoCardManager::DisableVideoCards()
//{
//    for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
//    {
//        m_VideoCards[i]->Disable();
//    }
//}
//
////**************************************
////
//void VideoCardManager::DisableVideoCard(int i)
//{
//    m_VideoCards[i]->Disable();
//}

////**************************************
////
//bool VideoCardManager::InitVideoCards   ( const std::vector<int> & hackBuffersUids )
//{
//    bool result = false;
//    for(unsigned int i =    0   ;   i < m_VideoCards.size() ;   i++)
//    {
//        //m_VideoCards[i]->SuperMagic = this->m_SuperMagic;
//        m_VideoCards[i]->mode = this->m_dislpayMode;
//
//        result = m_VideoCards[i]->InitVideoCard( hackBuffersUids );
//    }
//    //result = ((BlueFishVideoCard*)m_VideoCards[0])->InitDuplexPlaybacklayback();
//    return result;
//}

////**************************************
////
//bool VideoCardManager::InitVideoCard( int i, const std::vector<int> & hackBuffersUids )
//{
//    bool result = false;
//    m_VideoCards[i]->SuperMagic = this->m_SuperMagic;
//    m_VideoCards[i]->mode = this->m_dislpayMode;
//
//    result = m_VideoCards[i]->InitVideoCard( hackBuffersUids );
//    return result;
//}

////**************************************
////
//size_t VideoCardManager::GetVideoCardsSize()
//{
//    return m_videoCards.size();
//}
//
////**************************************
////
//unsigned char * VideoCardManager::GetCaptureBufferForShaderProccessing( unsigned int /*VideCardID*/, std::string /*ChannelName*//*A,B,C,D,E,F*/ )
//{
//    //return GetVideoCard(VideCardID)->GetCaptureBufferForShaderProccessing(ChannelName);
//    return nullptr;
//}
//  
//bool VideoCardManager::CheckIfNewFrameArrived                  (unsigned int /*VideCardID*/, std::string /*ChannelName*//*A,B,C,D,E,F*/)
//{
//    //return GetVideoCard(VideCardID)->CheckIfNewFrameArrived(ChannelName);
//    return false;
//}
//void   VideoCardManager::UnblockCaptureQueue                     (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/)
//{
//    return GetVideoCard(VideCardID)->UnblockCaptureQueue(ChannelName);
//}
//**************************************
//
//bool VideoCardManager::UpdateReferenceMode( unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/ )
//{
//    return GetVideoCard(VideoCardID)->UpdateReferenceMode( ChannelName, ReferenceModeName );
//}
//
////**************************************
////
//bool VideoCardManager::UpdateReferenceOffset( unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV )
//{
//    return GetVideoCard(VideoCardID)->UpdateReferenceOffset( ChannelName, refH, refV );
//}

} //videocards
} //bv