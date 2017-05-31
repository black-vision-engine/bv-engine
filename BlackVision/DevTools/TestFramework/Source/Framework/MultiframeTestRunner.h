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
public:
    explicit MultiframeTestRunner( TestReporter& reporter );
    ~MultiframeTestRunner();


    bool        ContinueTest    ( Test * curTest )
    {
        bv::FrameworkTest * const frameworkTest = dynamic_cast< bv::FrameworkTest * const >( curTest );
        
        // One frame test doesn't continue.
        if( !frameworkTest )
            return false;

        if( frameworkTest->IsLastFrame() )
            return false;

        if( m_result->GetFailureCount() > 0 )
            return false;

        return true;
    }

    int         RunSingleTest   ( Test * curTest, char const* /*suiteName*/, int maxTestTimeInMs )
    {
        RunTest( m_result, curTest, maxTestTimeInMs );

        if( !ContinueTest( curTest ) )
            return Finish();

        return m_result->GetFailureCount();
    }

    TestResults* GetTestResults();

private:
    TestReporter* m_reporter;
    TestResults* m_result;
    Timer* m_timer;

    int Finish() const;
    bool IsTestInSuite( const Test* const curTest, char const* suiteName ) const;
    void RunTest( TestResults* const result, Test* const curTest, int const maxTestTimeInMs ) const;
    void RunTestMulti( TestResults* const result, Test* const curTest, int const maxTestTimeInMs ) const;
};

}

#endif
