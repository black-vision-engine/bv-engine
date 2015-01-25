#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#include <string>
#include <glm/glm.hpp>


namespace bv {

class VertexShader : public Shader
{
public:

    VertexShader            ( const std::string & programSource, ShaderParameters * params );

};

} // bv
