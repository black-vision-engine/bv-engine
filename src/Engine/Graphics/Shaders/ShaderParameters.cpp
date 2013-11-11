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
void    ShaderParameters::RegisterParameter                 ( GenericShaderParam * param )
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
GenericShaderParam * ShaderParameters::GetParam             ( int id )
{
    assert( id >= 0 );
    assert( id < NumParameters() );

    return m_shaderParams[ id ];
}

// ***********************
//
GenericShaderParam * ShaderParameters::GetParam             ( const std::string & name )
{
    return FindParam( name );
}

// ***********************
//
const std::vector< Texture * > &  ShaderParameters::Textures() const
{
    return m_textures;
}

// ***********************
//
void                 ShaderParameters::AddTexture           ( Texture * tex )
{
    m_textures.push_back( tex );
}

// ***********************
//
void                ShaderParameters::SetValue              ( const std::string & name , const char * pBuf )
{
    GetParam( name )->SetValue( pBuf );
}

// ***********************
//FIXME: add some argumentsc if any argumetns are required
void                 ShaderParameters::Update               ()
{
    for ( auto param : m_shaderParams )
    {
        param->Update();
    }
}

// ***********************
//
GenericShaderParam * ShaderParameters::FindParam            ( const std::string & name ) const
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
