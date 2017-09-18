#include "Framework/FrameworkTest.h"


#include "ShaderCompiler.h"

#include "System/Path.h"


UNITTEST_SUITE( ShadersCompilation )
{


// ***********************
// This test will compile all fullscreen effect shaders.
class TestFullscreenShaders : public bv::FrameworkTest
{
private:
public:
    TestFullscreenShaders() : bv::FrameworkTest( "TestFullscreenShaders", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;

} TestFullscreenShadersInstance;

UnitTest::ListAdder adderTestFullscreenShaders ( UnitTest::Test::GetTestList(), &TestFullscreenShadersInstance );



using namespace bv;


// ***********************
//
void        TestFullscreenShaders::PreEvents           ()
{
    std::string vsPath = "Assets/Shaders/FullscreenEffects/fse_default.vert";

    std::string vsSource = File::Read( vsPath );
    auto psShaders = Path::List( "Assets/Shaders/FullscreenEffects/", true, ".*\\.frag" );

    for( auto & psPath : psShaders )
    {
        TestCompilation( vsPath, psPath.Str() );
    }
}



}

