#include "TestExecutor.h"

#include <fstream>


namespace bv
{


// ***********************
// Note: initialization order matters.
TestExecutor::TestExecutor      ( BVTestAppLogic * appLogic, UnitTest::Test * test, const std::string& filePath )
    :   m_resultFile( File::Open( filePath, File::OpenMode::FOMReadWrite ) )
    ,   m_reporter( *m_resultFile.StreamBuf() )
    ,   m_runner( m_reporter )
    ,   m_testList( test )
    ,   m_curTest( test )
    ,   m_appLogic( appLogic )
    ,   m_failedTests( 0 )
{}

// ***********************
//
TestExecutor::~TestExecutor()
{
    m_resultFile.Close();
}

// ***********************
//
bool        TestExecutor::WantContinue      ( UnitTest::Test * curTest )
{
    bv::FrameworkTest * const frameworkTest = dynamic_cast< bv::FrameworkTest * const >( curTest );

    // One frame test doesn't continue.
    if( !frameworkTest )
        return false;

    if( frameworkTest->IsLastFrame() )
        return false;

    if( m_runner.GetTestResults()->GetFailedTestCount() > m_failedTests )
        return false;

    return true;
}

// ***********************
//
bool        TestExecutor::Execute           ()
{
    if( !WantContinue( m_curTest ) )
    {
        // No tests anymore.
        if( m_testList == nullptr )
        {
            m_runner.Finish();
            return false;
        }

        m_curTest = FetchNextTest();
        m_failedTests = m_runner.GetTestResults()->GetFailedTestCount();
    }

    m_runner.RunSingleTest( m_curTest, nullptr, 0 );
    return true;
}

// ***********************
//
UnitTest::Test *    TestExecutor::FetchNextTest     ()
{
    // Make dynamic cast to check if this is really FrameworkTest. This could be class derived from UnitTest::Test
    // but not from FrameworkTest. In such a case simply ignore test and fetch next.
    UnitTest::Test * curTest = m_testList;
    FrameworkTest * frameworkTest = dynamic_cast< FrameworkTest * >( m_testList );
    m_testList = m_testList->m_nextTest;


    if( frameworkTest )
        frameworkTest->SetAppLogic( m_appLogic );

    curTest->m_nextTest = nullptr;
    return curTest;
}


}	// bv


