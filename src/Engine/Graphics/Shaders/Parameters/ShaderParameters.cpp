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
void               ShaderParameters::AddTexture             ( Texture2D * texture )
{
    m_textures.push_back( texture );
}

} //bv
