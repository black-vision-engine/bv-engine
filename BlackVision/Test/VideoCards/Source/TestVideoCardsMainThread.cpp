#include "TestVideoCardsMainThread.h"

#include "TestVideoCardsUtils.h"

#include "VideoCardManager.h"

#include "System/Time.h"

#include "UseLoggerTestVideoCardsModule.h"

namespace bv { namespace videocards {

const std::string   CONFIG_PATH = "config.xml";

// ****************************
//
TestVideoCardsMainThread::TestVideoCardsMainThread( AVFramePtr testFrame )
    : m_testFrame( testFrame )
    , m_vcm( VideoCardManager::Instance() )
{
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
    if( m_testFrame )
    {
        auto data = std::const_pointer_cast< MemoryChunk >( m_testFrame->m_videoData )->GetWritable();

        memset( data, rand(), m_testFrame->m_videoData->Size() );

        m_vcm.QueueFrame( m_testFrame );

        LOG_MESSAGE( SeverityLevel::info ) << Time::Now();
    }
    else
    {
        m_testFrame = TestVideoCardsUtils::CreateTestFrame( rand(), 1920, 1080 );
        m_vcm.QueueFrame( m_testFrame );
    }
    

}


} // videocards
} // bv