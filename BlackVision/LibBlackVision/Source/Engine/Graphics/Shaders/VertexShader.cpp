#include "stdafx.h"

#include "VertexShader.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
VertexShader::VertexShader( const std::string & programSource, ShaderParameters * params )
    : Shader( programSource, params )
{
}

} //bv
