#pragma once

#include "Engine/Graphics/Shaders/Shader.h"

#include <string>
#include <glm/glm.hpp>


namespace bv {

class VertexShader : public Shader
{
public:

    void        UpdateMVP   ( const glm::mat4 & MVP );
    void        UpdateMV    ( const glm::mat4 & MV );
    void        UpdateP     ( const glm::mat4 & MV );

    VertexShader            ( const std::string & programSource );

};

class PassThroughVertexShader : public VertexShader
{
    static std::string m_programSource;

public:

    PassThroughVertexShader();

};

} // bv
