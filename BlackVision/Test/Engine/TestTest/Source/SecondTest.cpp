#include "UnitTest++.h"

#include "Framework/BVTestAppLogic.h"
#include "Framework/FrameworkTest.h"


using namespace bv;


SUITE( FrameworkTests )
{


// ***********************
//
class SecondTest : public bv::FrameworkTest
{
private:
public:
    SecondTest() : bv::FrameworkTest( "SecondTest", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;
} SecondTestInstance;

UnitTest::ListAdder adderSecondTest ( UnitTest::Test::GetTestList(), &SecondTestInstance );


// ***********************
//
void        SecondTest::PreEvents     ()
{
    if( GetFrameNumber() == 0 )
    {
        PathVec paths;
        paths.push_back( Path( "witek/ExtrudeTest.scn" ) );

        GetAppLogic()->LoadScenes( paths );

        EndTestAfterThisFrame( false );
    }

    if( GetFrameNumber() >= 100 )
    {
        EndTestAfterThisFrame( true );
        GetAppLogic()->UnloadScenes();
    }
}


}
