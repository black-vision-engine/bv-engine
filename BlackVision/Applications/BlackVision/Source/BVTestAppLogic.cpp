#include "stdafxBVApp.h"

#include "BVTestAppLogic.h"

#include "Application/ApplicationContext.h"

#include "UnitTest++.h"
#include "TestReporterStdout.h"
#include "XmlTestReporter.h"

namespace bv
{

// *********************************
//
BVTestAppLogic::BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
    : BVAppLogic( renderer, audioRenderer )
{}

// *********************************
//
BVTestAppLogic::~BVTestAppLogic     ()
{}

// *********************************
//
void    BVTestAppLogic::OnUpdate    ( Renderer * , audio::AudioRenderer * )
{
    auto tl = UnitTest::Test::GetTestList();

    UnitTest::TestList selectedTests;

    tl.GetHead()->m_nextTest = nullptr;

    selectedTests.Add( tl.GetHead() );

    auto f = File::Open( tl.GetHead()->m_details.testName, File::OpenMode::FOMReadWrite );

    UnitTest::XmlTestReporter reporter( *f.StreamBuf() );
    UnitTest::TestRunner runner( reporter );
    runner.RunTestsIf( selectedTests, 0, UnitTest::True(), 0 );

    f.Close();

    // Empty. Updating in test framework is called manually.
}


// *********************************
//
void    BVTestAppLogic::LoadScene   ()
{
    // Empty. Test  Framework doesn't load any scene by default.
}

// *********************************
//
void    BVTestAppLogic::Update      ( unsigned int millis )
{
    m_frameStartTime = millis;

    ApplicationContext::Instance().IncrementUpdateCounter();

    GetDefaultEventManager().Update( DefaultConfig.EventLoopUpdateMillis() );

    ApplicationContext::Instance().IncrementUpdateCounter();

    TimeType time = m_renderMode.StartFrame( m_frameStartTime );

    HandleFrame( time, m_renderer, m_audioRenderer );
}

} //bv
