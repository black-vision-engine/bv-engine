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
AVFramePtr          VideoCardManager::KILLER_FRAME = nullptr;

//**************************************
//
VideoCardManager::VideoCardManager()
	: m_keyActive(true)
	, m_interlaceEnabled(true)
	, m_dislpayMode(DisplayMode::HD)
	, m_currentFrameNumber(0)
	, m_PreviousFrame(NULL)
	, m_InterlaceProducesFullFrames(false)
    , m_processThread( std::unique_ptr< VideoCardProcessingThread >( new VideoCardProcessingThread() ) )
	, m_numReadyCards( 0 )
{
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
                        videocard->SetFrameProcessingCompletedCallback( FrameProcessingCompleted );
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
void                        VideoCardManager::QueueFrame            (AVFramePtr data )
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
		short int odd = m_currentFrameNumber % 2;
		m_currentFrameNumber++;
		//if( data->m_desc.fieldModeEnabled && !m_InterlaceProducesFullFrames)
		//{
		//	data = RetrieveFieldFromFrame( data, odd );
		//}
		//else
		//{
  //          data = InterlacedFrame( data );
  //      }

		odd = m_currentFrameNumber % 2;

        m_numReadyCards = 0;
        for( auto & videoCard : m_videoCards )
        {
            videoCard->ProcessFrame( data );
        }

        {
            std::unique_lock< std::mutex > lock( m_mutex );
            m_waitFramesProcessed.wait( lock, [ = ] { return m_numReadyCards == 1; }); // TODO: check num of cards properly./

            for( auto & videoCard : m_videoCards )
            {
                videoCard->DisplayFrame();
            }
        }

        return true;
    }

    return false;
}

// *********************************
//
void               VideoCardManager::FrameProcessingCompleted( UInt64 deviceID, bool success )
{
    static VideoCardManager & instance = VideoCardManager::Instance();
    success;deviceID;
    instance.m_numReadyCards++;
    instance.m_waitFramesProcessed.notify_one();

    //instance.m_videoCards[ deviceID ]->DisplayFrame();
}

// *********************************
//
AVFramePtr         VideoCardManager::RetrieveFieldFromFrame(AVFramePtr frame, int odd)
{
	

	// poni¿sza funkcja wycina z [data] co Nt¹ b¹dŸ co N+1¹ liniê (zamiast pe³nej ramki przekazujemy pó³pole, zamiast InterlacedFrame powinno byæ bardziej coœ w stylu ConvertProgressiveFrameToField

	const char *mem_src = frame->m_videoData->Get();

	int pixel_depth = frame->m_desc.depth;  // pobraæ poni¿sze informacje (wdepth,  width, height z configa, albo niech tu nie przychodzi RawData tylko jakoœ to opakowane w klasê typu Frame
	int width = frame->m_desc.width;
	int height = frame->m_desc.height;
	int bytes_per_line = width * pixel_depth;

	int size = width * height/2 * pixel_depth + 2048; // z jakiegos powodu trzeba dodawaæ 2048 bajtów  poniewa¿ funkcja Bluefisha CalculateGoldenValue () zwraca tyle bajtów dla pó³pola HD, trzeab sprawdziæ jak to bedzie wygl¹daæ w SD

	char *mem_dst = new char[size];  // pewnie nie ma co tutaj tego za kazdym razem tworzyæ...

	for (int i = odd, j = 0;i < height;i += 2, j++)
	{
		memcpy(&mem_dst[j*(bytes_per_line)], &mem_src[i*(bytes_per_line)], bytes_per_line);
	}

	MemoryChunkConstPtr ptr = MemoryChunkConstPtr(new MemoryChunk((char*)mem_dst, size));  // ponownie - pewnie nie ma co tego tutaj tworzyæ za ka¿dym razem...

	frame->m_videoData = ptr;

	return frame;
}

// *********************************
//
AVFramePtr         VideoCardManager::InterlacedFrame(AVFramePtr frame)
{
	int pixel_depth = frame->m_desc.depth;  // pobraæ poni¿sze informacje (wdepth,  width, height z configa, albo niech tu nie przychodzi RawData tylko jakoœ to opakowane w klasê typu Frame
	int width = frame->m_desc.width;
	int height = frame->m_desc.height;
	int bytes_per_line = width * pixel_depth;
	int size = width * height * pixel_depth;

	const char *mem_new = frame->m_videoData->Get();
	
	if (m_PreviousFrame == NULL)
	{
		
		m_PreviousFrame = new char[size];
	}
	
	char *mem_dst = new char[size];

	for (int i = 0;i < height;i++)
	{
		if(i%2==1)
			memcpy(&mem_dst[i*(bytes_per_line)], &m_PreviousFrame[i*(bytes_per_line)], bytes_per_line);
		else
			memcpy(&mem_dst[i*(bytes_per_line)], &mem_new[i*(bytes_per_line)], bytes_per_line);
	}

	// yet to be implemented
	
	memcpy(m_PreviousFrame, mem_new, size);

	MemoryChunkConstPtr ptr = MemoryChunkConstPtr(new MemoryChunk((char*)mem_dst, size));  // ponownie - pewnie nie ma co tego tutaj tworzyæ za ka¿dym razem...
	frame->m_videoData = ptr;

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

} //videocards
} //bv