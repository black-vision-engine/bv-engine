#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#include <string>
#include <glm/glm.hpp>


namespace bv {

class GeometryShader : public Shader
{
public:

    GeometryShader( const std::string & programSource, ShaderParameters * params );

};

} // bv