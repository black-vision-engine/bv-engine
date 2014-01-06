#pragma once

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Models/Plugins/Parameters/TypedValues.h"


namespace bv {

class VertexShader : public Shader
{
private:

    model::ValueMat4Ptr         m_MVP;
    model::ValueMat4Ptr         m_MV;
    model::ValueMat4Ptr         m_P;

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
