#include "WireframeShaders.h"

#include <sstream>

#include "System/Path.h"
#include "IO/FileIO.h"


namespace  bv {

namespace {

// *********************************
//
std::string     EffectShadersDir()
{
    return "Assets/Shaders/Effects/";
}

// *********************************
//
std::string     ReadShaderContentsFromFile  ( const std::string & fileName )
{
    std::stringstream shaderSource;

    File::Open( fileName ) >> shaderSource;

    return shaderSource.str();
}

} // anonymous

// ****************************
//
std::string GetWireframePixelShaderCode     ()
{
    return ReadShaderContentsFromFile( EffectShadersDir() + "wireframe.frag" );
}

// ****************************
//
std::string GetWireframeVertexShaderCode    ()
{
    return ReadShaderContentsFromFile( EffectShadersDir() + "wireframe.vert" );
}

// ****************************
//
std::string GetWireframeGeometryShaderCode  ()
{
    return ReadShaderContentsFromFile( EffectShadersDir() + "wireframe.geom" );
}

} //bv
