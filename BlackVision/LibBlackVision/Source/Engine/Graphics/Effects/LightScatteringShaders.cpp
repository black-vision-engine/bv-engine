#include "stdafx.h"

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
std::string GetLightScatteringPixelShaderCode     ()
{
    return ReadShaderContentsFromFile( EffectShadersDir() + "light_scattering.frag" );
}

// ****************************
//
std::string GetLightScatteringVertexShaderCode    ()
{
    return ReadShaderContentsFromFile( EffectShadersDir() + "light_scattering.vert" );
}

// ****************************
//
//std::string GetLightScatteringGeometryShaderCode  ()
//{
//    return ReadShaderContentsFromFile( EffectShadersDir() + "light_scattering.geom" );
//}

} //bv
