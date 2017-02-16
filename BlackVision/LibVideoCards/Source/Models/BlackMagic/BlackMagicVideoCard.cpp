#include "BlackMagicVideoCard.h"

#include "Serialization/SerializationHelper.h"

#include "UseLoggerVideoModule.h"

#include "System/Time.h"

#include <chrono>

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

		bool result = card->InitOutput();
		{result;}

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
    , m_decklinkOutput( nullptr )
    , m_configuration( nullptr )
	, m_keyer( nullptr )
	, m_videoOutputDelegate( nullptr )
	, m_frameQueue( 1 )
	//, m_converToYUVNeeded( false )
{
    InitVideoCard();
}

//**************************************
//
VideoCard::~VideoCard       ()
{
	if( m_decklinkOutput )
	{
		m_decklinkOutput->StopScheduledPlayback( 0, NULL, 0 );

		m_frameQueue.EnqueueEndMessage();

		m_decklinkOutput->DisableVideoOutput();
		m_decklinkOutput->Release();
	}		

    if( m_configuration )
        m_configuration->Release();

    if( m_device )
        m_device->Release();

	if( m_videoOutputDelegate )
		m_videoOutputDelegate->Release();
}

//**************************************
//
bool                    VideoCard::InitVideoCard        ()
{
    if( InitDevice() )
    {
		LOG_MESSAGE( SeverityLevel::info ) << "BlackMagic device '" << m_deviceID << "' properly initialized.";
		return true;
    }
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannor properly initilized BlackMagic device '" << m_deviceID << "'.";
		return false;
	}
}

//**************************************
//
bool                    VideoCard::InitDevice           ()
{
    IDeckLinkIterator * iterator;

	bool success = false;

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

            if( 
				SUCCESS( m_device->QueryInterface( IID_IDeckLinkOutput, ( void** )&m_decklinkOutput ) ) &&
                SUCCESS( m_device->QueryInterface( IID_IDeckLinkConfiguration, ( void** )&m_configuration ) )
				)
            {
				LOG_MESSAGE( SeverityLevel::info ) << "Initilizing Decklink device output success. Device ID: " << m_deviceID;
				success = true;
            }
			else
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Cannot initilize Decklink device output. Device ID: " << m_deviceID;
				success = false;
			}
        }

        iterator->Release();
    }
	else
	{
		success = false;
	}

	if( success )
	{
		m_videoOutputDelegate = new VideoOutputDelegate( this );
		if( !SUCCESS( m_decklinkOutput->SetScheduledFrameCompletionCallback( m_videoOutputDelegate ) ) )
		{
			LOG_MESSAGE( SeverityLevel::error ) << "SetScheduledFrameCompletionCallback returned an error";
			success = false;
		}
	}

    return success;
}

//**************************************
//
bool                    VideoCard::InitOutput()
{
    IDeckLinkDisplayModeIterator * displayModeIterator = nullptr;
    m_displayMode = nullptr;

	bool success = false;

    if( SUCCESS( m_decklinkOutput->GetDisplayModeIterator( &displayModeIterator ) ) )
    {
        while( SUCCESS( displayModeIterator->Next( &m_displayMode ) ) )
        {
            if( m_displayMode->GetDisplayMode() == m_output.videoMode )
            {
                break;
            }

			m_displayMode->Release();
        }

        displayModeIterator->Release();

        if( m_displayMode )
        {
            if( SUCCESS( m_decklinkOutput->EnableVideoOutput( m_displayMode->GetDisplayMode(),
                                                        BMDVideoOutputFlags::bmdVideoOutputFlagDefault ) ) )
            {
				success &= InitKeyer( m_output );

				m_displayMode->GetFrameRate( &m_frameDuration, &m_frameTimescale );
            }
        }
    }
    
    return success;
}

//**************************************
//
bool					VideoCard::InitKeyer			( const ChannelOutputData & ch )
{
	if( ch.type == IOType::KEY || ch.type == IOType::FILL_KEY )
	{
		if( SUCCESS( m_device->QueryInterface( IID_IDeckLinkKeyer, ( void** ) &m_keyer ) ) )
		{
			m_keyer->Enable( true );
			m_keyer->SetLevel( 255 ); // Blend key completely onto the frame.
			return true;
		}
		else
		{
			LOG_MESSAGE( SeverityLevel::error ) << "Cannot obtain the IDeckLinkKeyer interface.";
			return false;
		}
	}
	else
	{
		return true;
	}
}

//**************************************
//
void                    VideoCard::SetVideoOutput       ( bool enable )
{
	m_output.enabled = enable;
}

//**************************************
//
void                    VideoCard::AddOutput            ( ChannelOutputData output )
{
	m_output = output;
}

//**************************************
//
void                    VideoCard::Start                ()
{
    m_uiTotalFrames = 0;

    IDeckLinkMutableVideoFrame * pFrame;

    auto w = m_displayMode->GetWidth();
    auto h = m_displayMode->GetHeight();

	BMDPixelFormat displayFormat = BMDPixelFormat::bmdFormat8BitBGRA;

	// Set 4 frame preroll
	for( unsigned i = 0; i < 4; i++ )
	{
		if( SUCCESS( m_decklinkOutput->CreateVideoFrame( w, h, w * 4, displayFormat,
														 bmdFrameFlagFlipVertical, &pFrame ) ) )
		{
			if( SUCCESS( m_decklinkOutput->ScheduleVideoFrame( pFrame, ( m_uiTotalFrames * m_frameDuration ), m_frameDuration, m_frameTimescale ) ) )
			{
				pFrame->Release();
				pFrame = nullptr;

				m_uiTotalFrames++;
			}
		}
	}


	m_decklinkOutput->StartScheduledPlayback( 0, m_frameTimescale, 1.0 );
}

//**************************************
//
void                    VideoCard::SetFrameProcessingCompletedCallback( FrameProcessingCompletedCallbackType callback )
{
    m_frameProcessingCompletedCallback = callback;
}

//**************************************
//
void                    VideoCard::ProcessFrame         ( AVFramePtr avFrame )
{
	//if( data->m_desc.fieldModeEnabled && !m_InterlaceProducesFullFrames)
	//{
	//	data = RetrieveFieldFromFrame( data, odd );
	//}
	//InterlaceFrame( avFrame );

	//if( odd == 0 )
	//{
		if( m_output.enabled )
		{
			m_frameQueue.WaitAndPush( avFrame );
		}
	//}

	auto nextSync = GetFrameTime() + 20;

	UpdateFrameTime( nextSync );

							   
	//auto sleepFor = nextSync - Time::Now();

	//if( sleepFor > 0 )
	//{
	//	std::this_thread::sleep_for( std::chrono::milliseconds( sleepFor ) );

	//	//LOG_MESSAGE( SeverityLevel::debug ) << "VideoCard::ProcessFrame: Slept for " << sleepFor << " miliseconds";
	//}
}

// *********************************
//
void			VideoCard::InterlaceFrame( AVFramePtr frame )
{
	int pixel_depth = frame->m_desc.depth;  // pobraæ poni¿sze informacje (wdepth,  width, height z configa, albo niech tu nie przychodzi RawData tylko jakoœ to opakowane w klasê typu Frame
	int width = frame->m_desc.width;
	int height = frame->m_desc.height;
	int bytes_per_line = width * pixel_depth;
	int size = width * height * pixel_depth;

	const char * memNew = frame->m_videoData->Get();

	if( !m_prevFrame )
	{
		m_prevFrame = MemoryChunk::Create( size );
	}

	char * memDst = m_prevFrame->GetWritable();

	for( int i = 0; i < height; i++ )
	{
		if( i % 2 == 1 )
			memcpy( &memDst[ i*( bytes_per_line ) ], &memDst[ i*( bytes_per_line ) ], bytes_per_line );
		else
			memcpy( &memDst[ i*( bytes_per_line ) ], &memNew[ i*( bytes_per_line ) ], bytes_per_line );
	}

	// yet to be implemented

	memcpy( memDst, memNew, size );

	MemoryChunkConstPtr ptr = MemoryChunkConstPtr( new MemoryChunk( ( char* ) memDst, size ) );  // ponownie - pewnie nie ma co tego tutaj tworzyæ za ka¿dym razem...
	frame->m_videoData = ptr;
}

//**************************************
//
UInt32                  VideoCard::AvailableVideoCards = EnumerateDevices();

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

            if(deckLink!=nullptr)
                deckLink->Release();
        }
    }

    return ( UInt32 )deviceCount;
}

//**************************************
//
void                            VideoCard::FrameCompleted       ( IDeckLinkVideoFrame * complitedFrame )
{
	DisplayNextFrame( complitedFrame );
}

//**************************************
//
void                            VideoCard::UpdateFrameTime      ( UInt64 t )
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_lastFrameTime = t;
}

//**************************************
//
UInt64                          VideoCard::GetFrameTime         () const
{
    std::unique_lock< std::mutex > lock( m_mutex );
    return m_lastFrameTime;
}

void                            VideoCard::DisplayFrame         () const
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_waitDisplay.notify_one();
}

//**************************************
//
void                            VideoCard::DisplayNextFrame     ( IDeckLinkVideoFrame * completedFrame )
{
    UpdateFrameTime( Time::Now() );

    AVFramePtr srcFrame;

    if( m_frameQueue.TryPop( srcFrame ) )
    {
        void * rawFrame;
        completedFrame->GetBytes( &rawFrame );
        
        memcpy( rawFrame, srcFrame->m_videoData->Get(), completedFrame->GetRowBytes() * completedFrame->GetHeight() );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::info ) << "Frame dropped on video output.";
    }

    m_frameProcessingCompletedCallback( m_deviceID, true );

    std::unique_lock< std::mutex > lock( m_mutex );
    m_waitDisplay.wait( lock );

    if( !SUCCESS( m_decklinkOutput->ScheduleVideoFrame( completedFrame, ( m_uiTotalFrames * m_frameDuration ), m_frameDuration, m_frameTimescale ) ) )
    {
        LOG_MESSAGE( SeverityLevel::info ) << "Cannot schedule frame.";
    }

    m_uiTotalFrames++;
}

} //blackmagic
} //videocards
} //bv

