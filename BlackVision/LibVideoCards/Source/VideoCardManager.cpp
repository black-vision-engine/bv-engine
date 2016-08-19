#include "VideoCardManager.h"
#include "Tools/HRTimer.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
//#include <..\dep\vld\include\vld.h>

#include "Models/BlackMagic/BlackMagicVideoCard.h"
//#include "Models/BlackMagic/BlackMagicVideoCardDescriptor.h"
#include "Models/BlueFish/BlueFishVideoCard.h"
//#include "Models/BlackMagic/BlueFishVideoCardDescriptor.h"

#include "UseLoggerVideoModule.h"


namespace bv { namespace videocards {


//**************************************
//
std::vector< IVideoCardDesc * >  DefaultVideoCardDescriptors  ()
{
    std::vector< IVideoCardDesc * > descriptors;
    
    //descriptors.push_back( new BlackMagic::VideoCardDesc() );
    //descriptors.push_back( new BlueFish::VideoCardDesc() );

    return descriptors;
}


//**************************************
//
VideoCardManager VideoCardManager::m_instance;


const UInt32    VideoCardManager::FRAMES_COUNT  = 2;
const UInt32    VideoCardManager::FHD           = 1920 * 1080 * 4;
const UInt32    VideoCardManager::WIDTH_BYTES   = 1920 * 4;


//**************************************
//
VideoCardManager::VideoCardManager      ()
    : m_enabled( false )
    , m_enableInterlace( false )
    , m_keyActive( true )
    , m_dislpayMode( DisplayMode::HD )
{
    m_processingThread = std::unique_ptr< ProcessingThread >( new ProcessingThread( this ) );
}

//**************************************
//
VideoCardManager::~VideoCardManager     ()
{
    for( auto desc : m_descVec )
    {
        delete desc;
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
bool                        VideoCardManager::IsEnabled             () const
{
    return m_enabled;
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
    m_enabled = true;
 //   printf("VideoCard INFO Starting video playback");

    if( GetVideoCardsSize() > 0)
    {
        m_processingThread->Start( Thread::ThreadPriotity::TIME_CRITICAL );
    }

  //  for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
  //  {
  //      m_VideoCards[i]->Enable();
        //m_VideoCards[i]->StartVideoCardProccessing();
  //  }
}

// *********************************
//
void                        VideoCardManager::ProcessBuffer         ()
{
    //static unsigned char buf[ fhd * frames_count ];
    //static unsigned int cur_buf = 0;

    MemoryChunkConstPtr data = nullptr;
    //m_dataQueue.WaitAndPop( data );
    while( !m_dataQueue.TryPop( data ) );

    auto frameBuf = ( unsigned char * )data->Get();

    //if( m_enableInterlace )
    //{
    //  unsigned int next_buf = ( cur_buf + 1 ) % FRAMES_COUNT;

    //  memcpy( &buf[ next_buf * fhd ], frameBuf, fhd );       
    //  unsigned char * prevFrameBuf = &buf[ cur_buf * fhd ];

    //  for( unsigned int i = 0;  i < 1080; i += 2 )
    //  {
    //      unsigned int cur_i = i + 1;
    //      unsigned int prev_i = i + 1;

    //      unsigned int cur_scanline = WIDTH_BYTES * cur_i;
    //      unsigned int prev_scanline = WIDTH_BYTES * prev_i;

    //      memcpy(&frameBuf[ cur_scanline ], &prevFrameBuf[ prev_scanline ], WIDTH_BYTES );

    //  }
    //}
    //cur_buf = ( cur_buf + 1 ) % frames_count; 

    if( !m_keyActive )
    {
        for( UInt32 i = 0; i < FHD; i += 4 )
        {
            /*[ +i+0]=0;
            frameBuf[ cur_scanline+i+1]=0;
            frameBuf[ cur_scanline+i+2]=0;*/
            frameBuf[ i + 3 ] = 0;
        }
    }
       
    DeliverFrameFromRAM( frameBuf ); 
}

// *********************************
//
IVideoCardPtr               VideoCardManager::CreateVideoCard       ( const std::string & uid, const IDeserializer & deser )
{
    auto desc = m_descMap.find( uid )->second;
    if( desc )
    {
        return desc->CreateVideoCard();
    }

    return nullptr;
}

// *********************************
//
const VideoCardManager &    VideoCardManager::Instance             ()
{
    return m_instance;
}

// *********************************
//
void                        VideoCardManager::ReadConfig            ( const IDeserializer & deser )
{
    if( deser.EnterChild( "videocard" ) )
    {
        do
        {
            auto name = deser.GetAttribute( "name" );
            if( IsRegistered( name ) )
            {
                auto videocard = 

                m_videoCards.push_back( videocard );
            }
        } while( deser.NextChild() );

        deser.ExitChild(); //videocard
    }
}

































//**************************************
//
VideoCardManager::VideoCardManager(void)
{
    m_SuperMagic=false;
    m_VideoCardConfig = VideoConfig();
    
}

//**************************************
//
VideoCardManager::~VideoCardManager(void)
{   
    m_enabled = false;
    //if( m_midgardThreadStopping == false )
    {
        //m_Midgard->PushKillerFrame();
        //m_Midgard->PushKillerFrame();

        //m_midgardThreadStopping = TRUE;
        //StopMidgardThread();
        
        cout << "Deleting videoCards.... " << endl;
        for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
        {
            m_VideoCards[i]->Disable();
        }

        for(auto &it:m_VideoCards) delete it; 
        m_VideoCards.clear();

        cout << "VideoCardManager deleted" << endl;
        //system("pause");
    }

}

//**************************************
//
bool VideoCardManager::StopMidgardThread()
{
    //if(m_midgardThreadHandle)
    {
        DWORD dw = 0;
        cout << "Stopping Midgard Thread..." << endl;
        //dw = WaitForSingleObject(m_midgardThreadHandle, 0);
        //CloseHandle(m_midgardThreadHandle);
        //m_midgardThreadHandle = NULL;
    }
    return true;
}


//**************************************
//


bool VideoCardManager::InitVideoCardManager(const std::vector<int> & hackBuffersUids)
{
    SetupVideoChannels();
    if(GetVideoCardsSize() > 0)
    {        
        InitVideoCards( hackBuffersUids );
    
        LOG_MESSAGE( SeverityLevel::info ) << "Detected " + to_string( GetVideoCardsSize() ) + " videocard(s)";
        
        return true;
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "NO VIDEO CARDS DETECTED";
        return false;
    }
}


//**************************************
//
void VideoCardManager::Enable()
{
    for( UInt32 i = 0; i < m_VideoCards.size(); ++i )
    {
        m_VideoCards[i]->Enable();
    }
}

//**************************************
//
void VideoCardManager::Disable()
{
    for( UInt32 i = 0; i < m_VideoCards.size(); ++i )
    {
        m_VideoCards[i]->Black();
        m_VideoCards[i]->Disable();
    }
}

//**************************************
//
void VideoCardManager::SetupVideoChannels()
{
    unsigned int bln = 0;
    for(unsigned int i = 0 ; i < m_VideoCards.size() ; i++)
    {
        if( m_VideoCards[i]->GetBrand() == "BlueFish" )
        {
            if( i > m_VideoCardConfig.blueFishCount) continue;
            bln++;
            if(m_VideoCards[i]->DetectOutputs() < m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount)
            {
                printf("VideoCards ERROR Too many Channels to configure");                
                return;
            }



            for(unsigned int z = 0; z < m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount;z++)
            {
                ((BlueFishVideoCard*)m_VideoCards[i])->AddChannel(m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].name,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.type,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].renderer,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.resolution,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refresh,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.interlaced,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.flipped,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].playback,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].capture,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.playthrough,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.type,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.referenceMode,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refH,
                    m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refV);
            }
        }
        else if(m_VideoCards[i]->GetBrand() == "BlackMagic")
        {
            //m_VideoCards[i]->SetReferenceModeValue(m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.referenceMode);
            //m_VideoCards[i]->SetReferenceOffsetValue(m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.refH,m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.refV);
        }
    }  
}
//**************************************
//
void VideoCardManager::ReadConfig()
{
    if( !m_VideoCardConfig.ReadbackFlag )
    {
        return;
    }

    if(m_VideoCardConfig.BlueFish)
    {
        RegisterBlueFishCards();
    }

    if(m_VideoCardConfig.BlackMagic && false)
    {
        RegisterBlackMagicCards();
        if(m_VideoCardConfig.superMagic)
        {
            m_SuperMagic=true;
        }
    }

    if(m_VideoCards.size()<=0)
        return;


    if(m_VideoCardConfig.resolutionOld=="SD")
    {
        m_dislpayMode =  DisplayMode::SD;
    }
}


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


//**************************************
//
void VideoCardManager::RegisterBlueFishCards()
{
    //CBlueVelvet4Ptr pSDK( BlueVelvetFactory4() );

    int numCards=0;
    CBlueVelvet4* BlueFishSDK = BlueVelvetFactory4();
    BlueFishSDK->device_enumerate(numCards);

    for(int i=1; i<=numCards ;i++)
    {
        m_VideoCards.push_back((VideoCardBase*) new BlueFishVideoCard(i));
    }
    BlueVelvetDestroy(BlueFishSDK);
    printf("VideoCards INFO Registered BlueFish video cards: %d \n", numCards);
}
//**************************************
//
void VideoCardManager::RegisterBlackMagicCards()
{
    int numCards = 0;
    //IDeckLink *deckLink=NULL;
    //CoInitialize(NULL);
    //IDeckLinkIterator* pDLIterator = NULL;    
    //CoCreateInstance(CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, (void**)&pDLIterator);
    //while (pDLIterator->Next(&deckLink) == S_OK)
   // {
        m_VideoCards.push_back((VideoCardBase*) new BlackMagicVideoCard(numCards+1));
        numCards++; 
    //}


    //pDLIterator->Release();

    printf("VideoCards INFO Registered Black Magic video cards: %d \n", numCards);
}
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

//**************************************
//
void VideoCardManager::PushDataFromRenderer     ( MemoryChunkConstPtr data )
{
    m_dataQueue.Push( data );
}

//**************************************
//
void VideoCardManager::DeliverFrameFromRAM(unsigned char * buffer)
{
    for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
    {
        //if(m_VideoCards[i]->IsActive())
            m_VideoCards[i]->DeliverFrameFromRAM(buffer);
    }    
}
//
////**************************************
////
//void VideoCardManager::DeliverFrameFromRAM(shared_ptr<CFrame> buffer)
//{
//  if(m_VideoCards[0]!=nullptr)
//  {        
//      for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
//      {
//          //if(m_VideoCards[i]->IsActive())
//              m_VideoCards[i]->DeliverFrameFromRAM(buffer);
//      }    
//  }
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

//**************************************
//
void VideoCardManager::Black()
{
    //DO NOT USE WHILE CLOSING APP OR WHILE DELETING VIDEOCARDS MANAGER
    for(unsigned int i = 0   ;   i < m_VideoCards.size() ; i++)
    {
        m_VideoCards[i]->Black();
    }
}


//**************************************
//
bool VideoCardManager::InitVideoCards   ( const std::vector<int> & hackBuffersUids )
{
    bool result = false;
    for(unsigned int i =    0   ;   i < m_VideoCards.size() ;   i++)
    {
        m_VideoCards[i]->SuperMagic = this->m_SuperMagic;
        m_VideoCards[i]->mode = this->m_dislpayMode;

        result = m_VideoCards[i]->InitVideoCard( hackBuffersUids );
    }
    //result = ((BlueFishVideoCard*)m_VideoCards[0])->InitDuplexPlaybacklayback();
    return result;
}

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

//**************************************
//
VideoCardBase* VideoCardManager::GetVideoCard(int i)
{
    return m_VideoCards[i];
}

//**************************************
//
size_t VideoCardManager::GetVideoCardsSize()
{
    return m_VideoCards.size();
}

//**************************************
//
unsigned char * VideoCardManager::GetCaptureBufferForShaderProccessing( unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/ )
{
    return GetVideoCard(VideCardID)->GetCaptureBufferForShaderProccessing(ChannelName);
}
  
bool VideoCardManager::CheckIfNewFrameArrived                  (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/)
{
    return GetVideoCard(VideCardID)->CheckIfNewFrameArrived(ChannelName);
}
void   VideoCardManager::UnblockCaptureQueue                     (unsigned int VideCardID, std::string ChannelName/*A,B,C,D,E,F*/)
{
    return GetVideoCard(VideCardID)->UnblockCaptureQueue(ChannelName);
}
//**************************************
//
bool VideoCardManager::UpdateReferenceMode( unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, std::string ReferenceModeName/*FREERUN,IN_A,IN_B,ANALOG,GENLOCK*/ )
{
    return GetVideoCard(VideoCardID)->UpdateReferenceMode( ChannelName, ReferenceModeName );
}

//**************************************
//
bool VideoCardManager::UpdateReferenceOffset( unsigned int VideoCardID, std::string ChannelName/*A,B,C,D,E,F*/, int refH, int refV )
{
    return GetVideoCard(VideoCardID)->UpdateReferenceOffset( ChannelName, refH, refV );
}

} //videocards
} //bv