#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#include <string>
#include "Mathematics/glm_inc.h"


namespace bv {

class VertexShader : public Shader
{
public:

    VertexShader            ( const std::string & programSource, ShaderParameters * params );

};

} // bv
