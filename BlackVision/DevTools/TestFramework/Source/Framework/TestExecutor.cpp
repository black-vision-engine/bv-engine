#include "TestExecutor.h"

#include <fstream>


namespace bv
{


// ***********************
// Note: initialization order matters.
TestExecutor::TestExecutor      ( FrameworkTest * test )
    :   m_resultFile( File::Open( test->m_details.testName, File::OpenMode::FOMReadWrite ) )
    ,   m_reporter( *m_resultFile.StreamBuf() )
    ,   m_runner( m_reporter )
    ,   m_test( test )
{}

// ***********************
//
TestExecutor::~TestExecutor()
{
    m_resultFile.Close();
}

// ***********************
//
bool        TestExecutor::WantContinue      () const
{
    return !m_test->IsLastFrame();
}

// ***********************
//
void        TestExecutor::Execute           ()
{
    UnitTest::TestList selectedTests;

    selectedTests.Add( m_test );
    m_runner.RunTestsIf( selectedTests, 0, UnitTest::True(), 0 );
}


}	// bv


