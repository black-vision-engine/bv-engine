#include "Framework/FrameworkTest.h"

#include "ShaderCompiler.h"

#include "System/Path.h"





// ***********************
//
class TestCombinations : public bv::FrameworkTest
{
private:
public:
    TestCombinations() : bv::FrameworkTest( "TestCombinations", "Shaders.Compilation", __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;

} TestCombinationsInstance;

UnitTest::ListAdder adderTestCombinations ( UnitTest::Test::GetTestList(), &TestCombinationsInstance );





// ***********************
//
void        TestCombinations::PreEvents           ()
{
    auto vsShadersList = Path::List( "Assets/Shaders/Combinations/", true, ".*\\.vert" );

    for( auto & vsShaderPath : vsShadersList )
    {
        auto psPath = FindMatchingPS( vsShaderPath );
        TestCompilation( vsShaderPath.Str(), psPath.Str() );
    }
}


