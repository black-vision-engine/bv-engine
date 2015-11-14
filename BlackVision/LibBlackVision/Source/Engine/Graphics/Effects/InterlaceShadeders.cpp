#include "MaskChannelsShaders.h"

#include <sstream>

#include "System/Path.h"
#include "IO/FileIO.h"


namespace bv {

// FIXME: merge this file and WireframeShaders into one and name it something like CommonShaders*
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
std::string GetInterlacePixelShaderCode   ()
{
    return ReadShaderContentsFromFile( EffectShadersDir() + "interlace.frag" );
}

// ****************************
//
std::string GetInterlaceVertexShaderCode  ()
{
    return ReadShaderContentsFromFile( EffectShadersDir() + "interlace.vert" );
}

} // bv
