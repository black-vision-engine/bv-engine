#pragma once

#include "IO/FileIO.h"

#include "FrameworkTest.h"

#include "UnitTest++.h"
#include "TestReporterStdout.h"
#include "XmlTestReporter.h"

#include "MultiframeTestRunner.h"


namespace bv
{

// ***********************
//
class TestExecutor
{
private:
    
    // Note: Members order matters. Check initialization in constructor.
    File                            m_resultFile;
    UnitTest::XmlTestReporter       m_reporter;
    UnitTest::MultiframeTestRunner  m_runner;

    FrameworkTest*                  m_test;

public:

    TestExecutor        ( FrameworkTest* test );
    ~TestExecutor       ();


    bool        WantContinue    ();

    void        Execute         ();
};




}	// bv

