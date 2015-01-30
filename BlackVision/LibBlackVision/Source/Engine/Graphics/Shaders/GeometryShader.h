#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#include <string>
#include "Mathematics/glm_inc.h"


namespace bv {

class GeometryShader : public Shader
{
public:

    GeometryShader( const std::string & programSource, ShaderParameters * params );

};

} // bv