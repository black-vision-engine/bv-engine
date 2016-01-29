#include "stdafx.h"

#include "VertexShader.h"


namespace bv {

// *********************************
//
VertexShader::VertexShader( const std::string & programSource, ShaderParameters * params )
    : Shader( programSource, params )
{
}

} //bv
