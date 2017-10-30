#include "Framework/FrameworkTest.h"

#include "ShaderCompiler.h"

#include "System/Path.h"




// ***********************
// Compiles and links all shaders in Effects folder.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Shaders_Compilation, TestEffects )
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


