#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#include <string>
#include <glm/glm.hpp>


namespace bv {

class GeometryShader : public Shader
{
public:

    void        UpdateMVP   ( const glm::mat4 & MVP );
    void        UpdateMV    ( const glm::mat4 & MV );
    void        UpdateP     ( const glm::mat4 & P );

    GeometryShader( const std::string & programSource );

};

} // bv