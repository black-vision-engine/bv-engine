#include "GeometryShader.h"

#include "Engine/Models/Parameter.h"

namespace bv {

// *******************************
//
GeometryShader::GeometryShader(const std::string& programSource )
    : Shader( programSource )
{
    ShaderParameters* ret   = GetOrCreateShaderParameters();

    m_MVP                   = new ValueMat4("MVP");
    GenericShaderParam* genShaderParam = ShaderParamFactory::Get().Create( m_MVP->GetName(), m_MVP->GetParamType() );
    ret->RegisterParameter(genShaderParam);

    m_MV                    = new ValueMat4("MV");
    genShaderParam          = ShaderParamFactory::Get().Create( m_MV->GetName(), m_MV->GetParamType() );
    ret->RegisterParameter(genShaderParam);

    m_P                     = new ValueMat4("P");
    genShaderParam          = ShaderParamFactory::Get().Create( m_P->GetName(), m_P->GetParamType() );
    ret->RegisterParameter(genShaderParam);
}

void GeometryShader::UpdateMVP( const glm::mat4& MVP )
{
    GenericShaderParam* mvp = m_parameters->GetParam("MVP");
    assert(mvp);

    mvp->GenericSetVal(&MVP);
}

void GeometryShader::UpdateMV( const glm::mat4& MV )
{
    GenericShaderParam* mv = m_parameters->GetParam("MV");
    assert(mv);

    mv->GenericSetVal(&MV);
}

void GeometryShader::UpdateP( const glm::mat4& P )
{
    GenericShaderParam* p = m_parameters->GetParam("P");
    assert(p);

    p->GenericSetVal(&P);
}

} // bv