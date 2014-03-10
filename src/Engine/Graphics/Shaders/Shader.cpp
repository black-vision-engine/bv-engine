#include "Shader.h"

#include <cassert>


namespace bv {

// *********************************
//
Shader::Shader( const std::string & programSource, ShaderParameters * params )
    : m_programSurce( programSource )
    , m_parameters( params )
{
}

// *********************************
//
Shader::~Shader ()
{
    delete m_parameters;
}

// *********************************
//
ShaderParameters *  Shader::GetParameters()
{
    return m_parameters;
}

// *********************************
//
void                Shader::AddTextureSampler( const TextureSampler * sampler )
{
    m_textureSamplers.push_back( sampler );
}

// *********************************
//
ShaderParameters *  Shader::Parameters          ()
{
    return m_parameters;
}

// *********************************
//
const std::string & Shader::ProgramSource       () const
{
    return m_programSurce;
}

// *********************************
//
void                Shader::Update              ( RenderableEntity * renderable, Camera * camera )
{
    m_parameters->UpdateParameters( renderable, camera );
}

// *********************************
//
const std::vector< const TextureSampler * > &  Shader::Samplers     () const
{
    return m_textureSamplers;
}

} //bv
