#include "ShaderParameters.h"

#include <cassert>


namespace bv {

// *******************************
//
ShaderParameters::ShaderParameters                          ()
{
}

// *******************************
//
ShaderParameters::~ShaderParameters                         ()
{
    for( auto param : m_shaderParams )
    {
        delete param;
    }
}

// ***************************
//
void                ShaderParameters::UpdateParameters      ( RenderableEntity * renderable, Camera * camera )
{
    for( auto param : m_shaderParams )
    {
        param->Update( renderable, camera );
    }
}

// ***************************
//
void    ShaderParameters::AddParameter                      ( GenericShaderParam * param )
{
    m_shaderParams.push_back( param );
}

// ***************************
//
void               ShaderParameters::AddTexture             ( Texture2DPtr texture )
{
    m_textures.push_back( texture );
}

// ***************************
//
void              ShaderParameters::SetTexture              ( unsigned int idx, Texture2DPtr tex )
{
    assert( idx < m_textures.size() );

    m_textures[ idx ] = tex;
}

} //bv
