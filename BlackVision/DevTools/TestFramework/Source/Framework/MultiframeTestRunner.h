#ifndef UNITTEST_MULTIFRAMETESTRUNNER_H
#define UNITTEST_MULTIFRAMETESTRUNNER_H

#include "Test.h"
#include "TestList.h"
#include "CurrentTest.h"

#include "FrameworkTest.h"



namespace UnitTest {

class TestReporter;
class TestResults;
class Timer;



class UNITTEST_LINKAGE MultiframeTestRunner
{
private:

    mutable double      m_testTimeInMs;

public:
    explicit MultiframeTestRunner( TestReporter& reporter );
    ~MultiframeTestRunner();


    int             RunSingleTest   ( Test * curTest, char const* /*suiteName*/, int maxTestTimeInMs );
    TestResults*    GetTestResults  ();
    int             Finish          () const;

private:

    TestReporter*       m_reporter;
    TestResults*        m_result;
    Timer*              m_timer;

    bool        IsTestInSuite   ( const Test* const curTest, char const* suiteName ) const;
    void        RunTest         ( TestResults* const result, Test* const curTest, int const maxTestTimeInMs ) const;
    void        RunTestMulti    ( TestResults* const result, Test* const curTest, int const maxTestTimeInMs ) const;
};

}

#endif
