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
BVTestAppLogic::BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const std::string & testname )
    : BVAppLogic( renderer, audioRenderer )
    , m_testExecutor( nullptr )
{
    m_testExecutor = new TestExecutor( this, UnitTest::Test::GetTestList().GetHead(), testname );
}

// *********************************
//
BVTestAppLogic::BVTestAppLogic      ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
    : BVAppLogic( renderer, audioRenderer )
    , m_testExecutor( nullptr )
{
    m_testExecutor = new TestExecutor( this, UnitTest::Test::GetTestList().GetHead(), "Test", true );
}

// *********************************
//
BVTestAppLogic::~BVTestAppLogic     ()
{
    delete m_testExecutor;
}

// *********************************
//
void            BVTestAppLogic::OnUpdate    ( Renderer * , audio::AudioRenderer * )
{
    if( !m_testExecutor->Execute() )
        EndExecution();
}

// ***********************
//
void            BVTestAppLogic::EndExecution()
{
    // This function should close application and release everything.
    ChangeState( BVAppState::BVS_CLOSING );
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
