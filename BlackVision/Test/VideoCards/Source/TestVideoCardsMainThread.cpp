#include "TestVideoCardsMainThread.h"

#include "TestVideoCardsUtils.h"

#include "VideoCardManager.h"

#include "System/Time.h"

#include "CoreDEF.h"

#include "UseLoggerTestVideoCardsModule.h"


namespace bv { namespace videocards {

const std::string   CONFIG_PATH = "config.xml";
const SizeType      BUFFER_SIZE = 10;
// ****************************
//
TestVideoCardsMainThread::TestVideoCardsMainThread()
    : m_vcm( VideoCardManager::Instance() )
    , m_lastQueuedFrameTime( 0 )
    , m_buffer( BUFFER_SIZE )
{
    for( SizeType i = 0; i < BUFFER_SIZE; ++i )
    {
        m_buffer.push_back( TestVideoCardsUtils::CreateTestFrame( 0, 1920, 1080 ) );
    }

    m_vcm.RegisterDescriptors( videocards::DefaultVideoCardDescriptors() );


    m_deserializer.LoadFile( CONFIG_PATH );

    m_deserializer.Reset();
    m_deserializer.EnterChild( "config" );

    m_vcm.ReadConfig( m_deserializer );
    m_vcm.Start();
}

// ****************************
//
void    TestVideoCardsMainThread::Process ()
{
    auto frame = m_buffer.front();
    auto data = std::const_pointer_cast< MemoryChunk >( frame->m_videoData )->GetWritable();

    memset( data, rand(), frame->m_videoData->Size() );

    m_vcm.QueueFrame( frame );

    auto now = Time::Now();

    TestVideoCardsUtils::UpdateConsoleTitle( std::to_wstring( 1000 / ( now - m_lastQueuedFrameTime ) ) + L" FPS" );

    m_lastQueuedFrameTime = now;

    m_buffer.push_back( frame );
}


} // videocards
} // bv