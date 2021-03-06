#include "TestVideoCardsMainThread.h"

#include "TestVideoCardsUtils.h"

#include "VideoCardManager.h"

#include "System/Time.h"

#include "CoreDEF.h"

#include "UseLoggerTestVideoCardsModule.h"


namespace bv {
namespace videocards
{

const std::string   CONFIG_PATH = "TestConfigs/DefaultConfig.xml";
const SizeType      BUFFER_SIZE = 10;



// ****************************
//
TestVideoCardsMainThread::TestVideoCardsMainThread() 
    : m_lastQueuedFrameTime( 0 )
    , m_buffer( BUFFER_SIZE )
	, m_frameNum( 0 )
{
    for( SizeType i = 0; i < BUFFER_SIZE; ++i )
    {
        m_buffer.push_back( TestVideoCardsUtils::CreateTestFrame( 0, 1920, 1080 ) );
    }

    m_vcm = new VideoCardManager();
    m_vcm->GetFactory().RegisterDefaultCreators();

    m_deserializer.LoadFile( CONFIG_PATH );
    m_deserializer.Reset();
    m_deserializer.EnterChild( "config" );
    
    m_vcm->CreateVideoCards( m_vcm->GetFactory().ReadDescriptorsFromConfig( m_deserializer ) );

    m_vcm->Start();
}

// ****************************
//
void    TestVideoCardsMainThread::Process ()
{
    auto frame = m_buffer.front();
    auto data = std::const_pointer_cast< MemoryChunk >( frame->m_videoData )->GetWritable();

    memset( data, 0, frame->m_videoData->Size() );

	for( int i = 0; i < 30; ++i )
	{
		memset( &( data[ ( ( ( 30 * m_frameNum ) + i ) % 1080 ) * 1920 * 4 ] ), 255, 4 * 1920 );
	}

    auto input = std::shared_ptr< VCMInputData >( new VCMInputData() );

    input->SetAVFrame( 0, frame );
    input->SetAVFrame( 1, frame );
    input->SetAVFrame( 2, frame );

    m_vcm->Display( input );

    auto now = Time::Now();

    TestVideoCardsUtils::SetFPS( now - m_lastQueuedFrameTime );

    m_lastQueuedFrameTime = now;

    m_buffer.push_back( frame );

	m_frameNum++;

    if( m_frameNum >= 500 )
        this->Kill();
}

// ****************************
//
TestVideoCardsMainThread::~TestVideoCardsMainThread   ()
{
    m_vcm->Stop();

    delete m_vcm;
}

} // videocards
} // bv