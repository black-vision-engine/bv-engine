#include "ShaderParameters.h"

#include <cassert>


namespace bv {

// ***********************
//
ShaderParameters::~ShaderParameters                         ()
{
    for( auto param : m_shaderParams )
    {
        delete param;
    }
}

// ***********************
//
void    ShaderParameters::SetOwner                          ( Shader * owner )
{
    m_shader = owner;
}

// ***********************
//
bool    ShaderParameters::IsRegistered                      ( const std::string & name ) const
{
    return FindParam( name ) != nullptr;
}

// ***********************
//
void    ShaderParameters::RegisterParameter                 ( UniformShaderParam * param )
{
    m_shaderParams.push_back( param );
}

// ***********************
//
int     ShaderParameters::NumParameters                     () const
{
    return m_shaderParams.size();   
}

// ***********************
//
UniformShaderParam * ShaderParameters::GetParam             ( int id )
{
    assert( id >= 0 );
    assert( id < NumParameters() );

    return m_shaderParams[ id ];
}

// ***********************
//
UniformShaderParam * ShaderParameters::GetParam             ( const std::string & name )
{
    return FindParam( name );
}

// ***********************
//
const ShaderTextureParameters &   ShaderParameters::GetTextureParameters            () const
{
    return m_textureParams;
}

// ***********************
//
ShaderTextureParameters &         ShaderParameters::TextureParameters               ()
{
    return m_textureParams;
}

// ***********************
//
void                ShaderParameters::SetValue              ( const std::string & name , const char * pBuf )
{
    GetParam( name )->SetValue( pBuf );
}

// ***********************
//
UniformShaderParam * ShaderParameters::FindParam            ( const std::string & name ) const
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
