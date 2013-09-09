#include "ShaderParameters.h"
#include <cassert>

namespace bv {

// ***********************
//
void    ShaderParameters::SetOwner                        ( Shader * owner )
{
    m_shader = owner;
}

// ***********************
//
bool    ShaderParameters::IsRegistered            ( const std::string & name ) const
{
    return FindParam( name ) != nullptr;
}

// ***********************
//
void    ShaderParameters::RegisterParameter       ( GenericShaderParam * param )
{
    m_shaderParams.push_back( param );
}

// ***********************
//
int     ShaderParameters::NumParameters           () const
{
    return m_shaderParams.size();   
}

// ***********************
//
GenericShaderParam * ShaderParameters::GetParam   ( int id )
{
    assert( id >= 0 );
    assert( id < NumParameters() );

    return m_shaderParams[ id ];
}

// ***********************
//
GenericShaderParam * ShaderParameters::GetParam   ( const std::string & name )
{
    return FindParam( name );
}

// ***********************
//
void                ShaderParameters::SetValue            ( const std::string& name , const char* pBuf)
{
    GetParam(name)->SetValue(pBuf);
}

// ***********************
//
void                 ShaderParameters::Update     ()
{
    for ( auto param : m_shaderParams )
    {
        param->Update();
    }
}

// ***********************
//
ShaderParameters::~ShaderParameters               ()
{
    for( auto param : m_shaderParams )
    {
        delete param;
    }
}

// ***********************
//
GenericShaderParam * ShaderParameters::FindParam          ( const std::string & name ) const
{
    for( auto param : m_shaderParams )
    {
        if( param->Descriptor().Name() == name )
        {
            return param;
        }
    }

    return nullptr;    
}

}
