#pragma once

#include "IO/FileIO.h"

#include "FrameworkTest.h"

#include "UnitTest++.h"
#include "TestReporterStdout.h"
#include "XmlGTestReporter.h"

#include "MultiframeTestRunner.h"


namespace bv
{

class BVTestAppLogic;

// ***********************
//
class TestExecutor
{
private:
    
    // Note: Members order matters. Check initialization in constructor.
    File                            m_resultFile;
    UnitTest::TestReporter *        m_reporter;
    UnitTest::MultiframeTestRunner  m_runner;

    UnitTest::Test *                m_testList;
    UnitTest::Test *                m_curTest;

    int                             m_failedTests;

    BVTestAppLogic *                m_appLogic;

public:

    TestExecutor        ( BVTestAppLogic * appLogic, UnitTest::Test * test, const std::string& filePath );
    TestExecutor        ( BVTestAppLogic * appLogic, UnitTest::Test * test, const std::string& filePath, bool reportStdOut );
    ~TestExecutor       ();

    bool                Execute         ();

private:

    bool                WantContinue    ( UnitTest::Test * curTest );
    UnitTest::Test *    FetchNextTest   ();

};




}	// bv

