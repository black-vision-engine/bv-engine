#include "GeometryShader.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

// *******************************
//
GeometryShader::GeometryShader( const std::string & programSource )
    : Shader( programSource )
{
    ShaderParameters * ret  = GetOrCreateShaderParameters();

    m_MVP                   = ValueMat4Ptr( new ValueMat4( "MVP" ) );
    UniformShaderParam * uniShaderParam = ShaderParamFactory::Get().Create( m_MVP->GetName(), m_MVP->GetType() );
    ret->RegisterParameter( uniShaderParam );

    m_MV                    = ValueMat4Ptr( new ValueMat4( "MV" ) );
    uniShaderParam          = ShaderParamFactory::Get().Create( m_MV->GetName(), m_MV->GetType() );
    ret->RegisterParameter( uniShaderParam );

    m_P                     = ValueMat4Ptr( new ValueMat4( "P" ) );
    uniShaderParam          = ShaderParamFactory::Get().Create( m_P->GetName(), m_P->GetType() );
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
