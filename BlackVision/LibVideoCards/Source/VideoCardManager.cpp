#include "VideoCardManager.h"
#include <memory>
#include <iostream>
#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"

#include "Mathematics/Core/MathFuncs.h"

//#include "UseLoggerVideoModule.h"


namespace bv { namespace videocards {


VCMInputDataConstPtr VideoCardManager::KILLER_FRAME = nullptr;

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
				if (name == "BlackMagic")
				{
					m_InterlaceProducesFullFrames = true; // kinda hack - bluefish requires 50 x fields and blackamgic requires 25 x interlaced frames - they need different refresh rates and data size inside frame memory
				}

                if( IsRegistered( name ) )
                {
                    auto videocard = m_descMap[ name ]->CreateVideoCard( deser );
                    if( videocard )
                    {
                        m_videoCards.push_back( videocard );
						for( auto id : videocard->GetDisplayedVideoOutputsIDs() )
						{
							m_outputsToCardsMapping.insert( std::make_pair( id, videocard ) );
						}
                        //videocard->SetFrameProcessingCompletedCallback( FrameProcessingCompleted );
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
