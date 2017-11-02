#include "Framework/FrameworkTest.h"


#include "ShaderCompiler.h"

#include "System/Path.h"



using namespace bv;


// ***********************
// This test will compile all fullscreen effect shaders.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Shaders_Compilation, TestFullscreenShaders )
{
    std::string vsPath = "Assets/Shaders/FullscreenEffects/fse_default.vert";

    std::string vsSource = File::Read( vsPath );
    auto psShaders = Path::List( "Assets/Shaders/FullscreenEffects/", true, ".*\\.frag" );

    for( auto & psPath : psShaders )
    {
        TestCompilation( vsPath, psPath.Str() );
    }
}




