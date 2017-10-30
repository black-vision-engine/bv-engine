#include "Framework/FrameworkTest.h"

#include "ShaderCompiler.h"

#include "System/Path.h"





// ***********************
//
class TestEffects : public bv::FrameworkTest
{
private:
public:
    TestEffects() : bv::FrameworkTest( "TestEffects", "Shaders.Compilation", __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;

} TestEffectsInstance;

UnitTest::ListAdder adderTestEffects ( UnitTest::Test::GetTestList(), &TestEffectsInstance );





// ***********************
//
void        TestEffects::PreEvents           ()
{
    auto vsShadersList = Path::List( "Assets/Shaders/Effects/", true, ".*\\.vert" );

    for( auto & vsShaderPath : vsShadersList )
    {
        auto psPath = FindMatchingPS( vsShaderPath );
        auto gsPath = FindMatchingGS( vsShaderPath );

        if( Path::Exists( gsPath ) )
            TestCompilation( vsShaderPath.Str(), psPath.Str(), gsPath.Str() );
        else
            TestCompilation( vsShaderPath.Str(), psPath.Str() );
    }
}


