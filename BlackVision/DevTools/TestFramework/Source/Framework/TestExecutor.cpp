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
bool        TestExecutor::WantContinue      ()
{
    bool failed = m_runner.GetTestResults()->GetFailedTestCount() > 0;

    return !failed && !m_test->IsLastFrame();
}

// ***********************
//
void        TestExecutor::Execute           ()
{
    m_runner.RunSingleTest( m_test, nullptr, 0 );
}


}	// bv


