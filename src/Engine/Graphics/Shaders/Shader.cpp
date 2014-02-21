#include "Shader.h"

#include <cassert>

#include "Engine/Models/Updaters/ShaderParamUpdater.h"


namespace bv {

// *********************************
//
Shader::Shader( const std::string & programSource )
    : m_programSurce( programSource )
    , m_parameters( nullptr )
    , m_paramUpdater( nullptr )
{
}

// *********************************
//
Shader::~Shader ()
{
    delete m_paramUpdater;
    delete m_parameters;
}

// *********************************
//
ShaderParameters *  Shader::GetOrCreateShaderParameters()
{
    if( !m_parameters )
    {
        m_parameters = new ShaderParameters();
        m_parameters->SetOwner( this );
    }
    
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
void                Shader::RegisterUpdater  ( ShaderParamUpdater * updater )
{
    assert( !m_paramUpdater );
    
    m_paramUpdater = updater;
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
void                Shader::Update              ()
{
    //FIXME: ?????
    if( m_paramUpdater )
        m_paramUpdater->DoUpdate( 0.f );
}

// *********************************
//
const std::vector< const TextureSampler * > &  Shader::Samplers     () const
{
    return m_textureSamplers;
}

} //bv
