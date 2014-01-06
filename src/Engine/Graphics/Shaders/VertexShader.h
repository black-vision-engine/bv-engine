#pragma once

#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv {

class VertexShader : public Shader
{
    model::ValueMat4Ptr         m_MVP;
    model::ValueMat4Ptr         m_MV;
    model::ValueMat4Ptr         m_P;

public:

    void        UpdateMVP   ( const glm::mat4 & MVP );
    void        UpdateMV    ( const glm::mat4 & MV );
    void        UpdateP     ( const glm::mat4 & MV );

    VertexShader            ( const std::string & programSource );

};

class PassThroughVertexShaderParametersDescriptor
{
public:


};

class PassThroughVertexShader : public VertexShader
{
    static std::string m_programSource;

public:

    PassThroughVertexShader( PassThroughVertexShaderParametersDescriptor * desc );

};


} // bv
