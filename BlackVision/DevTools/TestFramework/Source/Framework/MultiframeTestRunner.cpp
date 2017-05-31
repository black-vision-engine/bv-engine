#include "MultiframeTestRunner.h"
#include "TestResults.h"
#include "TestReporter.h"
#include "TestReporterStdout.h"
#include "TimeHelpers.h"
#include "MemoryOutStream.h"

#include <cstring>

#include "FrameworkTest.h"


namespace UnitTest {


MultiframeTestRunner::MultiframeTestRunner( TestReporter& reporter )
    : m_reporter( &reporter )
    , m_result( new TestResults( &reporter ) )
    , m_timer( new Timer )
{
    m_timer->Start();
}

MultiframeTestRunner::~MultiframeTestRunner()
{
    delete m_result;
    delete m_timer;
}

TestResults* MultiframeTestRunner::GetTestResults()
{
    return m_result;
}

int MultiframeTestRunner::Finish() const
{
    float const secondsElapsed = static_cast<float>( m_timer->GetTimeInMs() / 1000.0 );
    m_reporter->ReportSummary( m_result->GetTotalTestCount(),
        m_result->GetFailedTestCount(),
        m_result->GetFailureCount(),
        secondsElapsed );

    return m_result->GetFailureCount();
}

bool MultiframeTestRunner::IsTestInSuite( const Test* const curTest, char const* suiteName ) const
{
    using namespace std;
    return ( suiteName == NULL ) || !strcmp( curTest->m_details.suiteName, suiteName );
}

void MultiframeTestRunner::RunTest( TestResults* const result, Test* const curTest, int const maxTestTimeInMs ) const
{
    bv::FrameworkTest * const frameworkTest = dynamic_cast< bv::FrameworkTest * const >( curTest );

    if( frameworkTest )
        RunTestMulti( result, curTest, maxTestTimeInMs );
    else
    {
        if( curTest->m_isMockTest == false )
            CurrentTest::Results() = result;

        Timer testTimer;
        testTimer.Start();

        result->OnTestStart( curTest->m_details );

        curTest->Run();

        double const testTimeInMs = testTimer.GetTimeInMs();
        if( maxTestTimeInMs > 0 && testTimeInMs > maxTestTimeInMs && !curTest->m_details.timeConstraintExempt )
        {
            MemoryOutStream stream;
            stream << "Global time constraint failed. Expected under " << maxTestTimeInMs <<
                "ms but took " << testTimeInMs << "ms.";

            result->OnTestFailure( curTest->m_details, stream.GetText() );
        }

        result->OnTestFinish( curTest->m_details, static_cast< float >( testTimeInMs / 1000.0 ) );
    }
}

void MultiframeTestRunner::RunTestMulti( TestResults * const result, Test * const curTest, int const maxTestTimeInMs ) const
{
    bv::FrameworkTest * const frameworkTest = dynamic_cast< bv::FrameworkTest * const >( curTest );

    if( curTest->m_isMockTest == false )
        CurrentTest::Results() = result;

    Timer testTimer;
    testTimer.Start();

    if( frameworkTest->GetFrameNumber() == 0 )
        result->OnTestStart( curTest->m_details );

    curTest->Run();

    double const testTimeInMs = testTimer.GetTimeInMs();
    if( maxTestTimeInMs > 0 && testTimeInMs > maxTestTimeInMs && !curTest->m_details.timeConstraintExempt )
    {
        MemoryOutStream stream;
        stream << "Global time constraint failed. Expected under " << maxTestTimeInMs <<
            "ms but took " << testTimeInMs << "ms.";

        result->OnTestFailure( curTest->m_details, stream.GetText() );
    }

    // Finish only if it was last frame.
    if( frameworkTest->IsLastFrame() )
        result->OnTestFinish( curTest->m_details, static_cast< float >( m_timer->GetTimeInMs() / 1000.0 ) );
}

}
