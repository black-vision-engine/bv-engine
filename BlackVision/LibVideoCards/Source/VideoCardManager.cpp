#include "VideoCardManager.h"
#include <memory>
#include <iostream>
#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"

#include "Mathematics/Core/MathFuncs.h"

//#include "UseLoggerVideoModule.h"


namespace bv {
namespace videocards
{


VCMInputDataConstPtr VideoCardManager::KILLER_FRAME = nullptr;




//**************************************
//
VideoCardProcessingThread::VideoCardProcessingThread    ( VideoCardManager * vcm )
    : m_running( true )
    , m_vcm( vcm )
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
        if( !m_vcm->ProcessOutputsData() )
        {
            break;
        }
    }
}

//**************************************
//
VideoCardManager::VideoCardManager()
	: m_keyActive(true)
	, m_interlaceEnabled(true)
	, m_dislpayMode(DisplayMode::HD)
	, m_currentFrameNumber(0)
	, m_PreviousFrame(NULL)
	, m_InterlaceProducesFullFrames(false)
	, m_numReadyCards( 0 )
{
    m_processThread = std::unique_ptr< VideoCardProcessingThread >( new VideoCardProcessingThread( this ) );
}

//**************************************
//
VideoCardManager::~VideoCardManager     ()
{
    // kill processing thread with killer frame
    Display( KILLER_FRAME );

    {
        std::unique_lock< std::mutex > lock( m_mutex );
        for( auto & videoCard : m_videoCards )
        {
            videoCard->SetVideoOutput( false );
        }
        m_videoCards.clear();
    }
}

// ***********************
//
void                        VideoCardManager::CreateVideoCards      ( const std::vector< IVideoCardDescPtr > & descriptors )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    for( auto & videoCardDesc : descriptors )
    {
        if( videoCardDesc->GetVideoCardUID() == "BlackMagic" )
        {
            m_InterlaceProducesFullFrames = true; // kinda hack - bluefish requires 50 x fields and blackamgic requires 25 x interlaced frames - they need different refresh rates and data size inside frame memory
        }

        auto videocard = videoCardDesc->CreateVideoCard();
        if( videocard )
        {
            m_videoCards.push_back( videocard );
            for( auto id : videocard->GetDisplayedVideoOutputsIDs() )
            {
                m_outputsToCardsMapping.insert( std::make_pair( id, videocard ) );
            }

            for( auto inputDesc : videocard->GetInputChannelsDescs() )
            {
                m_inputsToCardMapping.insert( std::make_pair( inputDesc.GetInputID(), videocard ) );
            }

            //videocard->SetFrameProcessingCompletedCallback( FrameProcessingCompleted );
        }
    }
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
        videoCard->PreStart();
    }

    for( auto & videoCard : m_videoCards )
    {
		videoCard->Start();
    }
}

//**************************************
//
void                        VideoCardManager::Stop                    ()
{
    std::unique_lock< std::mutex > lock( m_mutex );

    Display( KILLER_FRAME );

    for( auto & videoCard : m_videoCards )
    {
        videoCard->Stop();
    }
}

// *********************************
//
VCMInputData::VCMInputData()
{}

// *********************************
//
void			VCMInputData::SetAVFrame( UInt64 avOutputID, const AVFrameConstPtr & avFrame )
{
	m_outputsFramesMapping[ avOutputID ] = avFrame;
}

// *********************************
//
AVFrameConstPtr	VCMInputData::GetAVFrame( UInt64 avOutputID ) const
{
	auto it = m_outputsFramesMapping.find( avOutputID );

	if( it != m_outputsFramesMapping.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

// *********************************
//
void						 VideoCardManager::Display		        ( const VCMInputDataConstPtr & outputs )
{
	m_inputDataBuffer.Push( outputs );
}

// ***********************
//
AVFramePtr                  VideoCardManager::QueryInputFrame       ( VideoInputID inputID )
{
    auto iter = m_inputsToCardMapping.find( inputID );
    if( iter != m_inputsToCardMapping.end() )
    {
        return iter->second->QueryInputFrame( inputID );
    }

    return AVFramePtr();
}

// *********************************
//
bool                        VideoCardManager::ProcessOutputsData     ()
{
    auto outputs = m_inputDataBuffer.Pop();

    if( outputs )
    {
        short int odd = m_currentFrameNumber % 2;
        m_currentFrameNumber++;

        odd = m_currentFrameNumber % 2;

        for( auto it = outputs->begin(); it != outputs->end(); ++it )
        {
            auto cards = m_outputsToCardsMapping.equal_range( it->first );
            
            for( auto i = cards.first; i != cards.second; ++i )
            {
                i->second->ProcessFrame( it->second, it->first );
            }
        }
  
        return true;
    }

    return false;
}

// *********************************
//
UInt32                      VideoCardManager::GetRequiredFPS          () const
{
    UInt32 fps = 1;

    for( auto & vc : m_videoCards )
    {
        fps = ::bv::mathematics::lcm( vc->GetRequiredFPS(), fps );
    }

    return fps;
}

//// *********************************
////
//void               VideoCardManager::FrameProcessingCompleted( UInt64 deviceID, bool success )
//{
//    static VideoCardManager & instance = VideoCardManager::Instance();
//    success;deviceID;
//
//
//	std::unique_lock< std::mutex > lock( instance.m_mutex );
//    instance.m_numReadyCards++;
//    instance.m_waitFramesProcessed.notify_one();
//}

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

//// *********************************
////
//VideoCardManager &      VideoCardManager::Instance             ()
//{
//    static VideoCardManager instance;
//    return instance;
//}

} //videocards
} //bv
