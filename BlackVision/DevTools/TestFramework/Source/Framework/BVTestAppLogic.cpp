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
    , m_testExecutor( nullptr )
    , m_testList( nullptr )
{
    m_testList = UnitTest::Test::GetTestList().GetHead();
}

// *********************************
//
BVTestAppLogic::~BVTestAppLogic     ()
{}

// *********************************
//
void            BVTestAppLogic::OnUpdate    ( Renderer * , audio::AudioRenderer * )
{
    if( m_testExecutor )
        ContinueCurrentTest();
    else if( !BeginNewTest() )
        EndExecution();
}


// ***********************
//
void            BVTestAppLogic::ContinueCurrentTest()
{
    if( m_testExecutor->WantContinue() )
        m_testExecutor->Execute();
    else
    {
        delete m_testExecutor;
        m_testExecutor = nullptr;
    }
}

// ***********************
//
bool            BVTestAppLogic::BeginNewTest()
{
    if( m_testList == nullptr )
        return false;

    FrameworkTest * newTest = FetchNextTest( m_testList );

    if( newTest == nullptr )
        return false;

    newTest->SetAppLogic( this );

    assert( m_testExecutor == nullptr );
    m_testExecutor = new TestExecutor( newTest );
    
    // Always execute first loop. Test will decide if it needs more loops in ContinueCurrentTest function.
    m_testExecutor->Execute();
    return true;
}


// ***********************
//
FrameworkTest * BVTestAppLogic::FetchNextTest( UnitTest::Test * m_testsList )
{
    // Make dynamic cast to check if this is really FrameworkTest. This could be class derived from UnitTest::Test
    // but not from FrameworkTest. In such a case simply ignore test and fetch next.
    FrameworkTest * nextTest = dynamic_cast< FrameworkTest * >( m_testsList );
    m_testList = m_testList->m_nextTest;
    

    if( nextTest )
    {
        nextTest->m_nextTest = nullptr;
        return nextTest;
    }
    else
    {
        delete nextTest;
        return FetchNextTest( m_testList );
    }
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
