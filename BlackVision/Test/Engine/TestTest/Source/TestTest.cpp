#include "UnitTest++.h"

#include "Framework/BVTestAppLogic.h"
#include "Framework/FrameworkTest.h"


using namespace bv;


// ***********************
//
class TestTest : public bv::FrameworkTest
{
private:
public:
    TestTest() : bv::FrameworkTest( "TestTest", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}
    
    virtual void        PreEvents           () override;
} TestTestInstance;

UnitTest::ListAdder adderTestTest ( UnitTest::Test::GetTestList(), &TestTestInstance );


// ***********************
//
void        TestTest::PreEvents     ()
{
    if( GetFrameNumber() == 0 )
    {
        PathVec paths;
        paths.push_back( Path( "witek/NodeMaskPreview.scn" ) );

        GetAppLogic()->LoadScenes( paths );

        EndTestAfterThisFrame( false );
    }

    if( GetFrameNumber() >= 100 )
    {
        EndTestAfterThisFrame( true );
        GetAppLogic()->UnloadScenes();
    }
}
