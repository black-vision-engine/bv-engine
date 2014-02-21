#include "VertexShader.h"


namespace bv {

// *********************************
//
std::string PassThroughVertexShader::m_programSource = "   #version 400 \n \
                                                            \
                                                            layout (location = 0) in vec3 vertexPosition; \n \
                                                            \
                                                            uniform mat4 MVP; \n \
                                                            uniform mat4 MV; \n \
                                                            uniform mat4 P; \n \
                                                            mat4 dummy() \
                                                            {\
                                                                return MV * P * MVP;\
                                                            }\
                                                            \
                                                            void main() \n \
                                                            {\n \
                                                            dummy();\
                                                                gl_Position = MVP * vec4( vertexPosition, 1.0 );\n \
                                                            }\n \
                                                        ";


// *********************************
//
VertexShader::VertexShader( const std::string & programSource )
    : Shader( programSource )
{
    ShaderParameters * ret   = GetOrCreateShaderParameters();

    UniformShaderParam * uniShaderParam = ShaderParamFactory::Get().Create( "MVP", ParamType::PT_FLOAT4 );
    ret->RegisterParameter( uniShaderParam );

    uniShaderParam          = ShaderParamFactory::Get().Create( "MV", ParamType::PT_FLOAT4 );
    ret->RegisterParameter( uniShaderParam );

    uniShaderParam          = ShaderParamFactory::Get().Create( "P", ParamType::PT_FLOAT4 );
    ret->RegisterParameter( uniShaderParam );
}

// *********************************
//
void        VertexShader::UpdateMVP( const glm::mat4 & MVP )
{
    UniformShaderParam * mvp = m_parameters->GetParam( "MVP" );
    assert( mvp );

    mvp->GenericSetVal( &MVP );
}

// *********************************
//
void        VertexShader::UpdateMV( const glm::mat4 & MV )
{
    UniformShaderParam * mv = m_parameters->GetParam( "MV" );
    assert( mv );

    mv->GenericSetVal( &MV );
}

// *********************************
//
void        VertexShader::UpdateP( const glm::mat4 & P )
{
    UniformShaderParam * p = m_parameters->GetParam( "P" );
    assert( p );

    p->GenericSetVal( &P );
}

// ***************************** DESCRIPTOR **********************************

// *********************************
//
PassThroughVertexShader::PassThroughVertexShader()
    : VertexShader( PassThroughVertexShader::m_programSource )
{
    //FIXME: pass params in constructor
}



} //bv
