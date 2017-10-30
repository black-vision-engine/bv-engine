#include "Framework/FrameworkTest.h"

#include "ShaderCompiler.h"

#include "System/Path.h"




// ***********************
// Tries to compile and link all shaders in combination folder.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Shaders_Compilation, TestCombinations )
{
    auto vsShadersList = Path::List( "Assets/Shaders/Combinations/", true, ".*\\.vert" );

    for( auto & vsShaderPath : vsShadersList )
    {
        auto psPath = FindMatchingPS( vsShaderPath );
        TestCompilation( vsShaderPath.Str(), psPath.Str() );
    }
}


