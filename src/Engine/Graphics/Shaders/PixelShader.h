#pragma once

#include "Engine/Graphics/Shaders/Shader.h"


namespace bv {

class PixelShader : public Shader
{
public:

    PixelShader( const std::string & programSource, ShaderParameters * params );

};

} // bv
