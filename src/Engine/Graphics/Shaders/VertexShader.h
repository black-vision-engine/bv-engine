#pragma once

#include "Shader.h"

#include "Engine/Models/Parameter.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv {

namespace model
{
    class ValueMat4;
}

class VertexShader : public Shader
{
    model::ValueMat4*          m_MVP;
    model::ValueMat4*          m_MV;
    model::ValueMat4*          m_P;

public:
    void        UpdateMVP( const glm::mat4& MVP );
    void        UpdateMV( const glm::mat4& MV );
    void        UpdateP( const glm::mat4& MV );

    VertexShader( const std::string & programSource );

};

class PassThroughVertexShaderParametersDescriptor : public BaseParametersDescriptor
{
public:
    static const std::string pluginName;

    static const std::string MVPParamName;

    explicit PassThroughVertexShaderParametersDescriptor();
};

class PassThroughVertexShader : public VertexShader
{
    static std::string m_programSource;

public:

    PassThroughVertexShader(PassThroughVertexShaderParametersDescriptor* desc);

};


} // bv