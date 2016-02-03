#include "stdafx.h"

#include "GeometryShader.h"


namespace bv {

// *******************************
//
GeometryShader::GeometryShader( const std::string & programSource, ShaderParameters * params )
    : Shader( programSource, params )
{
}

} // bv
