#include "GeometryShader.h"


namespace bv {

// *******************************
//
GeometryShader::GeometryShader( const std::string & programSource )
    : Shader( programSource )
{
    ShaderParameters * ret  = GetOrCreateShaderParameters();

    UniformShaderParam * uniShaderParam = ShaderParamFactory::Get().Create( "MVP", ParamType::PT_FLOAT4 );
    ret->RegisterParameter( uniShaderParam );

    uniShaderParam          = ShaderParamFactory::Get().Create( "MV", ParamType::PT_FLOAT4 );
    ret->RegisterParameter( uniShaderParam );

    uniShaderParam          = ShaderParamFactory::Get().Create( "P", ParamType::PT_FLOAT4 );
    ret->RegisterParameter( uniShaderParam );
}

// *******************************
//
void GeometryShader::UpdateMVP( const glm::mat4 & MVP )
{
    UniformShaderParam * mvp = m_parameters->GetParam( "MVP" );
    assert( mvp );

    mvp->GenericSetVal( &MVP );
}

// *******************************
//
void GeometryShader::UpdateMV( const glm::mat4 & MV )
{
    UniformShaderParam * mv = m_parameters->GetParam( "MV" );
    assert( mv );

    mv->GenericSetVal( &MV );
}

// *******************************
//
void GeometryShader::UpdateP( const glm::mat4 & P )
{
    UniformShaderParam * p = m_parameters->GetParam( "P" );
    assert( p );

    p->GenericSetVal( &P );
}

} // bv
