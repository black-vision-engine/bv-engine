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
void            BVTestAppLogic::OnUpdate    ( Renderer * , audio::AudioRenderer * )
{
    auto testList = UnitTest::Test::GetTestList();

    UnitTest::TestList selectedTests;

    testList.GetHead()->m_nextTest = nullptr;

    selectedTests.Add( testList.GetHead() );

    auto f = File::Open( testList.GetHead()->m_details.testName, File::OpenMode::FOMReadWrite );

    UnitTest::XmlTestReporter reporter( *f.StreamBuf() );
    UnitTest::TestRunner runner( reporter );
    runner.RunTestsIf( selectedTests, 0, UnitTest::True(), 0 );

    f.Close();

     // Empty. Updating in test framework is called manually.
}


// *********************************
//
void            BVTestAppLogic::LoadScene   ()
{
    // Empty. Test  Framework doesn't load any scene by default.
}

// ***********************
//
void            BVTestAppLogic::PostFrameLogic()
{
    // Empty. Test Framework doesn't compute frame statistics.
}

} //bv
