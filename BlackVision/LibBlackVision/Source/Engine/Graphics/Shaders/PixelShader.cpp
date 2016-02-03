#include "stdafx.h"

#include "PixelShader.h"


namespace bv {

// ********************************
//
PixelShader::PixelShader( const std::string& programSource, ShaderParameters * params )
    : Shader( programSource, params )
{
}

} //bv
